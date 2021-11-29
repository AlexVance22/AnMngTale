#include "PCH.h"
#include "Scene.h"

#include "core/Asserts.h"
#include "core/Profiler.h"
#include "core/JsonCasts.h"

#include "gui/widgets/Button.h"
#include "gui/callbacks/CBCore.h"

#include "global/AudioManager.h"
#include "global/Data.h"

#include "entities/Player.h"


std::string f_to_string(float val, uint32_t prec)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(prec) << val;
	return stream.str();
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T>& vec)
{
	return stream << '[' << vec.x << ", " << vec.y << ']';
}


const char* freadall(const std::string& filepath)
{
	std::ifstream f(filepath, std::ios::binary);
	MNG_ASSERT_SLIM(f.is_open());
	f.seekg(0, std::ios::end);
	size_t size = f.tellg();
	f.seekg(0, std::ios::beg);
	char* buf = new char[size + 1];
	f.read(buf, size);
	f.close();
	buf[size] = '\0';

	return buf;
}

void loadjson(rapidjson::Document& doc, const std::string& filepath)
{
	const char* source = freadall(filepath);
	MNG_ASSERT_BASIC(!doc.Parse(source).HasParseError());
	delete[] source;
}

void dumpjson(const rapidjson::Document& doc, const std::string& filepath)
{
	std::ofstream stream(filepath, std::ios::trunc);
	MNG_ASSERT_SLIM(stream.is_open());
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	doc.Accept(writer);
	stream.write(sb.GetString(), sb.GetLength());
	stream.close();
}


/*
const std::unordered_map<Scene::AreaState, const char*> Scene::stateToStr{ 
	{ AreaState::INTRO, "intro" },
	{ AreaState::DEFAULT, "default" },
	{ AreaState::EXPO, "expo" },
	{ AreaState::BUILD, "build" },
	{ AreaState::CLIMAX, "climax" },
};
*/

sf::RenderWindow* Scene::p_window;
const float Scene::s_physScale = 1.f / 30.f;
sf::RenderTexture Scene::m_fadeTarget;


void Scene::handleGui(const float deltaTime)
{
	m_overlays.top().handleEvents();
	m_overlays.top().update(deltaTime);

	if (m_overlays.top().getNextScene())
	{
		m_nextScene = m_overlays.top().getNextScene();
		m_quit = true;
	}
	else if (m_overlays.top().getMasterQuit())
	{
		while (!m_overlays.empty())
			m_overlays.pop();
	}
	else if (m_overlays.top().getQuit())
		m_overlays.pop();
}


void Scene::loadCamera(const rapidjson::Value& data)
{
	const auto& pos = data["pos"];
	const auto& size = data["size"];
	const auto& constr = data["constraints"];
	m_camera.reset(sf::FloatRect(pos[0].GetFloat(), pos[1].GetFloat(),
								 size[0].GetFloat(), size[1].GetFloat()));
	m_camera.setConstraints(JsonToVec2<float>(constr[0]), JsonToVec2<float>(constr[1]));
	m_camera.enableConstraints(data["constrain"].IsTrue());
}

void Scene::loadGraphics(const rapidjson::Value& data)
{
	for (const auto& tex : data["textures"].GetArray())
		MNG_ASSERT_BASIC(m_textures[tex[0].GetString()].loadFromFile(tex[1].GetString()));
	for (const auto& sha : data["shaders"].GetArray())
	{
		MNG_ASSERT_BASIC(m_shaders[sha["name"].GetString()].loadFromFile(sha["path"].GetString(), (sf::Shader::Type)sha["type"].GetUint()));
		if (sha["texture"].IsTrue())
			m_shaders[sha["name"].GetString()].setUniform("texture", sf::Shader::CurrentTexture);
	}

	for (const auto& spr : data["sprites"].GetArray())
	{
		MatSprite* s = nullptr;
		const char* layer = spr["layer"].GetString();
		if (strncmp(layer, "bg", 8) == 0)
			s = &m_backgroundSprites.emplace_back();
		if (strncmp(layer, "fg", 8) == 0)
			s = &m_foregroundSprites.emplace_back();
		MNG_ASSERT_SLIM(s);

		s->sprite.setPosition(JsonToVec2<float>(spr["pos"]));
		MNG_ASSERT_SLIM(m_textures.find(spr["texture"].GetString()) != m_textures.end());
		s->sprite.setTexture(m_textures[spr["texture"].GetString()]);
		if (!spr["anim"].IsNull())
		{
			const auto& dims = spr["anim"]["dims"];
			s->sprite.animate(dims[0].GetUint(), dims[1].GetUint());
			s->sprite.setFrameRate(spr["anim"]["framerate"].GetFloat());
			s->sprite.setLoop(spr["anim"]["loop"].IsTrue());
			s->sprite.setIsPlaying(spr["anim"]["play"].IsTrue());
		}
		const auto& sha = spr["shader"];
		if (!sha.IsNull())
		{
			MNG_ASSERT_SLIM(m_shaders.find(sha.GetString()) != m_shaders.end());
			s->states.shader = &m_shaders[sha.GetString()];
		}
	}
}

void Scene::loadPhysics(const rapidjson::Value& data)
{
	const auto& statics = data["statics"];

	m_physWorld = std::make_unique<b2World>(b2Vec2(0, 0));

	for (const auto& box : statics["boxes"].GetArray())
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set((box[0].GetFloat() - box[2].GetFloat() * 0.5f) * s_physScale,
							 (box[1].GetFloat() - box[3].GetFloat() * 0.5f) * s_physScale);

		b2PolygonShape shape;
		shape.SetAsBox(box[2].GetFloat() * 0.5f * s_physScale, box[3].GetFloat() * 0.5f * s_physScale);

#ifdef MNG_DEBUG
		sf::RectangleShape debugShape;
		debugShape.setFillColor(sf::Color(0, 0, 0, 0));
		debugShape.setOutlineColor(sf::Color(255, 255, 255));
		debugShape.setOutlineThickness(0.3f);
		debugShape.setPosition(box[0].GetFloat() - box[2].GetFloat(), box[1].GetFloat() - box[3].GetFloat());
		debugShape.setSize(sf::Vector2f(box[2].GetFloat(), box[3].GetFloat()));
		d_debugBoxColliders.push_back(debugShape);
#endif

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = &shape;

		b2Body* body = m_physWorld->CreateBody(&bodyDef);
		MNG_ASSERT_SLIM(body);
		body->CreateFixture(&fixtureDef);
	}

	for (const auto& chain : statics["chains"].GetArray())
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0, 0);

		std::vector<b2Vec2> vertices;
		for (const auto& vert : chain["verts"].GetArray())
		{
			vertices.emplace_back(vert[0].GetFloat() * s_physScale, vert[1].GetFloat() * s_physScale);
#ifdef MNG_DEBUG
			d_debugChainColliders.append(JsonToVec2<float>(vert));
#endif
		}

		b2ChainShape shape;
		if (chain["loop"].IsTrue())
			shape.CreateLoop(vertices.data(), (uint32_t)vertices.size());
		else
		{
			const auto& ghosts = chain["ghosts"];
			shape.CreateChain(vertices.data(), (uint32_t)vertices.size(),
				b2Vec2(ghosts[0][0].GetFloat() * s_physScale, ghosts[0][1].GetFloat() * s_physScale),
				b2Vec2(ghosts[1][0].GetFloat() * s_physScale, ghosts[1][1].GetFloat() * s_physScale));
		}

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = &shape;

		b2Body* body = m_physWorld->CreateBody(&bodyDef);
		MNG_ASSERT_SLIM(body);
		body->CreateFixture(&fixtureDef);
	}

	for (const auto& bt : statics["box-triggers"].GetArray())
	{
		m_triggers[bt[0].GetString()] = BoxTrigger(JsonToRect(bt[1]));

#ifdef MNG_DEBUG
		sf::RectangleShape debugShape;
		debugShape.setFillColor(sf::Color(0, 0, 0, 0));
		debugShape.setOutlineColor(sf::Color(255, 255, 255));
		debugShape.setOutlineThickness(0.3f);
		debugShape.setPosition(JsonToVec2<float>(bt[1]));
		debugShape.setSize(sf::Vector2f(bt[1][2].GetFloat(), bt[1][3].GetFloat()));
		d_debugBoxColliders.push_back(debugShape);
#endif
	}
}

void Scene::loadSounds(const rapidjson::Value& data)
{
	if (data["reset"].IsTrue())
	{
		AudioManager::clearEffects();
		AudioManager::track.stop();

		const char* type = data["type"].GetString();

		if (strncmp(type, "music", 32) == 0)
			AudioManager::initTrack(data["file"].GetString(), true);
		else if (strncmp(type, "effect", 32) == 0)
			AudioManager::addEffectLayer(data["name"].GetString(), data["file"].GetString(), true).play();
		else
			MNG_ASSERT_MSG(false, "Invalid track type");
	}
}

void Scene::loadEntities(const rapidjson::Value& data)
{
	for (const auto& e : data.GetArray())
	{
		std::unique_ptr<Entity> entity = nullptr;

		const char* type = e["type"].GetString();
		if (strncmp(type, "player", 32) == 0)
		{
			MNG_ASSERT_SLIM(!m_player);
			entity = std::make_unique<Player>();
			entity->lock(e["locked"].IsTrue());
			m_player = dynamic_cast<Player*>(entity.get());
		}
		else if (strncmp(type, "character", 32) == 0)
			entity = std::make_unique<Entity>();
		else if (strncmp(type, "static", 32) == 0)
			entity = std::make_unique<Entity>();

		MNG_ASSERT_SLIM(entity);

		entity->setPosition(JsonToVec2<float>(e["pos"]));
		entity->setSize(JsonToVec2<float>(e["size"]));

		MNG_ASSERT_SLIM(m_textures.find(e["texture"].GetString()) != m_textures.end());

		const auto& anim = e["anim"];
		if (anim.IsNull())
			entity->m_sprite.setTexture(m_textures[e["texture"].GetString()]);
		else
		{
			entity->m_sprite.setTexture(m_textures[e["texture"].GetString()]);
			entity->m_sprite.animate(anim["frames"][0].GetUint(), anim["frames"][1].GetUint());
			entity->m_sprite.setFrameRate(anim["framerate"].GetFloat());
			entity->m_sprite.setLoop(true);
			entity->m_sprite.setIsPlaying(true);
		}

		if (e["camera-target"].IsTrue())
			m_camera.setDynamicTarget(&entity->getPosition());

		const auto& phys = e["physics"];
		if (!phys.IsNull())
		{
			b2BodyDef bodydef;
			const sf::Vector2f physPos = (entity->getPosition() + JsonToVec2<float>(phys["offset"])) * s_physScale;
			bodydef.position.Set(physPos.x, physPos.y);

			const char* type = phys["type"].GetString();
			if (strncmp(type, "static", 32) == 0)
				bodydef.type = b2_staticBody;
			else if (strncmp(type, "kinematic", 32) == 0)
				bodydef.type = b2_kinematicBody;
			else if (strncmp(type, "dynamic", 32) == 0)
				bodydef.type = b2_dynamicBody;
			else
				MNG_ASSERT_MSG(false, "Invalid body type");

			bodydef.fixedRotation = true;

			b2Body* body = m_physWorld->CreateBody(&bodydef);
			MNG_ASSERT_SLIM(body);

			b2FixtureDef fixturedef;
			fixturedef.density = phys["density"].GetFloat();

			const auto& shape = phys["shape"];
			const char* stype = shape["type"].GetString();
			if (strncmp(stype, "box", 32) == 0)
			{
				b2PolygonShape s;
				s.SetAsBox(shape["size"][0].GetFloat() * s_physScale * 0.5f, shape["size"][1].GetFloat() * s_physScale * 0.5f);
				fixturedef.shape = &s;
				body->CreateFixture(&fixturedef);

				const sf::FloatRect collider(
					phys["offset"][0].GetFloat(),
					phys["offset"][1].GetFloat(),
					shape["size"][0].GetFloat(),
					shape["size"][1].GetFloat()
				);

				entity->simulate(body, collider);
			}
			/*
			if (strncmp(stype, "polygon", 32) == 0)
			{
				b2PolygonShape s;
				//s.SetAsBox(shape["width"].GetFloat() * s_physScale * 0.5f, shape["height"].GetFloat() * s_physScale * 0.5f);
				fixturedef.shape = &s;
				body->CreateFixture(&fixturedef);
			}
			if (strncmp(stype, "chain", 32) == 0)
			{
				b2ChainShape s;
				//s.SetAsBox(shape["width"].GetFloat() * s_physScale * 0.5f, shape["height"].GetFloat() * s_physScale * 0.5f);
				fixturedef.shape = &s;
				body->CreateFixture(&fixturedef);
			}
			*/

			//entity->simulate(body);
		}

		Script::pushEntity(entity.get(), e["script-handle"].GetString());
		m_entities.push_back(std::move(entity));
	}
}

void Scene::loadParticles(const rapidjson::Value& data)
{
	for (const auto& psd : data.GetArray())
	{
		sf::FloatRect bounds = JsonToRect(psd["bounds"]);
		ParticleSystem& ps = m_particles.emplace_back(bounds);

		const char* mode = psd["mode"].GetString();

		if (strncmp(mode, "fireflies", 32) == 0)
		{
			ps.setMode(ParticleSystem::Mode::Fireflies);
			ps.generate(psd["init-count"].GetUint());
		}
		else if (strncmp(mode, "snow", 32) == 0)
		{
			ps.setMode(ParticleSystem::Mode::Snow);
			ps.setGenRate(psd["gen-rate"].GetFloat());
		}
		else if (strncmp(mode, "fountain", 32) == 0)
		{
			ps.setMode(ParticleSystem::Mode::Fountain);
			ps.setEmitter(JsonToVec2<float>(psd["emitter"]));
			ps.setStartVel(psd["start-vel"].GetFloat());
			ps.setGravityStr(psd["gravity-str"].GetFloat());
			ps.setGravityDir(psd["gravity-dir"].GetFloat());
			ps.setDragStr(psd["drag-str"].GetFloat());
			ps.setGenRate(psd["gen-rate"].GetFloat());
		}
		else
			MNG_ASSERT_MSG(false, "Invalid particle system mode");

		if (psd["rand-size"].IsNull())
			ps.setSize(JsonToVec2<float>(psd["base-size"]));
		else
			ps.setSize(JsonToVec2<float>(psd["base-size"]), JsonToVec2<float>(psd["rand-size"]));

		if (!psd["shrink-rate"].IsNull())
			ps.setShrinkRate(psd["shrink-rate"].GetFloat());

		ps.setColor(JsonToColor(psd["color"]));

		if (!psd["texture"].IsNull())
		{
			MNG_ASSERT_SLIM(m_textures.find(psd["texture"].GetString()) != m_textures.end());
			ps.setTexture(m_textures[psd["texture"].GetString()]);
		}
	}
}

void Scene::loadScripts(const rapidjson::Value& data)
{
	for (const auto& s : data.GetArray())
	{
		Script::compile(s.GetString());
		m_scripts.emplace_back(Script::suffix(s.GetString()), &m_camera);
	}
}


void Scene::reloadResources()
{
	const std::string filepath = "config/saves/save" + std::to_string(0) + "/" + m_name + ".json";
	rapidjson::Document doc;
	loadjson(doc, filepath);

	loadCamera(doc["camera"]);
	loadGraphics(doc["graphics"]);
	loadPhysics(doc["physics"]);
	loadSounds(doc["soundtrack"]);
	loadEntities(doc["entities"]);
	loadParticles(doc["particles"]);
	loadScripts(doc["scripts"]);

	/*
	render(&m_fadeTarget);
	for (int i = 0; i < 10; i++)
	{

	}
	*/
}


void Scene::baseEvents(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			m_overlays.emplace(p_window, &m_overlays, "res/menus/quit.json");
			m_overlays.top().getWidget<gui::Button>("quit")->onClick.bind(&popGame, &m_overlays);
			m_overlays.top().getWidget<gui::Button>("options")->onClick.bind(&pushOptionsMenu, p_window, &m_overlays);
			m_overlays.top().getWidget<gui::Button>("resume")->onClick.bind(&Menu::scheduleQuit, &m_overlays.top());
			break;
		case sf::Keyboard::E:
			m_overlays.emplace(p_window, &m_overlays, "res/menus/agenda.json");
			break;
		case sf::Keyboard::R:
#ifdef _DEBUG
			reloadResources();
#endif
			break;
		}
		break;
	case sf::Event::MouseButtonPressed:
#ifdef _DEBUG
		std::cout << p_window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_camera) << '\n';
#endif
		break;
	}
}


Scene::Scene(const std::string& scenename)
	: m_name(scenename), m_postfx(sf::Vector2u(1920, 1080))
	
#ifdef MNG_DEBUG
	, d_debugChainColliders(sf::LineStrip), d_debugBoxColliders(sf::Lines),
	d_framerate("Framerate: NA", d_debugFont, 18), d_tUpdate("Update:    NA", d_debugFont, 18),
	d_tRender("Render:    NA", d_debugFont, 18), d_tPhysics("Physics:   NA", d_debugFont, 18),
	d_tUserlogic("Logic:     NA", d_debugFont, 18)
#endif
{
	Script::reset();

	reloadResources();

	m_postfx.loadShader("res/shaders/blur.frag", "blur");
	m_postfx.setEnabled("blur", false);

#ifdef MNG_DEBUG
	d_debugFont.loadFromFile("res/fonts/courierprime.ttf");
	d_framerate.setPosition(10, 10);
	d_tUpdate.setPosition(10, 40);
	d_tRender.setPosition(10, 60);
	d_tPhysics.setPosition(10, 90);
	d_tUserlogic.setPosition(10, 110);
	d_framerate.setFillColor(sf::Color(0, 0, 0));
	d_tUpdate.setFillColor(sf::Color(0, 0, 0));
	d_tRender.setFillColor(sf::Color(0, 0, 0));
	d_tPhysics.setFillColor(sf::Color(0, 0, 0));
	d_tUserlogic.setFillColor(sf::Color(0, 0, 0));
#endif
}


void Scene::update(const float deltaTime)
{
	PROFILE_DEBUG_ONLY_BEGIN(false, MICROSECONDS);

	m_postfx.setEnabled("blur", !m_overlays.empty());

	for (auto& p : m_particles)
		p.update(deltaTime);

	if (m_overlays.empty())
	{
		handleEvents();

		for (auto& a : m_backgroundSprites)
			a.sprite.update(deltaTime);
		for (auto& e : m_entities)
			e->update(deltaTime);
		for (auto& a : m_foregroundSprites)
			a.sprite.update(deltaTime);

		for(auto& s : m_scripts)
			s.update(deltaTime);

		impl(deltaTime);

		PROFILE_DEBUG_ONLY_STEP();

		m_physWorld->Step(deltaTime, 6, 2);

		std::sort(m_entities.begin(), m_entities.end(), [](const std::unique_ptr<Entity>& left, const std::unique_ptr<Entity>& right)
			{
				return left->getPosition().y + left->getSize().y < right->getPosition().y + right->getSize().y;
			});

		m_camera.update(deltaTime);

		if (m_player)
		{
			const sf::FloatRect collider = m_player->getCollider();

			for (auto [_, t] : m_triggers)
				t.triggerOnIntersects(collider);
		}

		PROFILE_DEBUG_ONLY_STEP();

#ifdef MNG_DEBUG
		const auto& times = __prof.data();
		d_tUpdate.setString("Update:    " + f_to_string((times[0] + times[1]) * 0.001f, 3) + "ms");
		d_tPhysics.setString("Physics:   " + f_to_string(times[0] * 0.001f, 3) + "ms");
		d_tUserlogic.setString("Logic:     " + f_to_string(times[1] * 0.001f, 3) + "ms");
#endif
	}
	else
	{
		handleGui(deltaTime);

		PROFILE_DEBUG_ONLY_STEP();

#ifdef MNG_DEBUG
		const std::string t = f_to_string(__prof.data()[0] * 0.001f, 3) + "ms";
		d_tUpdate.setString("Update:    " + t);
		d_tPhysics.setString("Physics:   NA");
		d_tUserlogic.setString("Logic:     " + t);
#endif
	}

#ifdef MNG_DEBUG
	d_framerate.setString("Framerate: " + std::to_string((uint32_t)(1.f / deltaTime)));
#endif
}

void Scene::render(sf::RenderTarget* target)
{
	PROFILE_DEBUG_ONLY_BEGIN(false, MICROSECONDS);

	if (!target)
		target = p_window;

	m_postfx.setView(m_camera);

	for (const auto& sp : m_backgroundSprites)
		m_postfx.draw(sp.sprite, sp.states);
	for (const auto& e : m_entities)
		m_postfx.draw(*e);
	for (const auto& sp : m_foregroundSprites)
		m_postfx.draw(sp.sprite, sp.states);

#ifdef MNG_DEBUG
	m_postfx.draw(d_debugChainColliders);
	for (const auto& rect : d_debugBoxColliders)
		m_postfx.draw(rect);
#endif

	if (m_postfx.guiIncluded)
	{
		if (!m_overlays.empty())
			m_overlays.top().render(&m_postfx);

		m_postfx.render(target);
	}
	else
	{
		m_postfx.render(target);

		if (!m_overlays.empty())
			m_overlays.top().render(target);

		target->setView(sf::View(sf::FloatRect(0, 0, 1920, 1080)));
		for (const auto& p : m_particles)
			target->draw(p);
	}

	PROFILE_DEBUG_ONLY_STEP();

#ifdef MNG_DEBUG
	d_tRender.setString("Render:    " + f_to_string(__prof.data()[0] * 0.001f, 3) + "ms");
	target->draw(d_framerate);
	target->draw(d_tUpdate);
	target->draw(d_tRender);
	target->draw(d_tPhysics);
	target->draw(d_tUserlogic);
#endif
}