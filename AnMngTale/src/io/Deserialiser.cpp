#include "PCH.h"
#include "Deserialiser.h"

#include "JsonUtils.h"

#include "core/Scene.h"
#include "core/Asserts.h"
#include "entities/Player.h"
#include "global/AudioManager.h"

#define LOG_F(x) std::cout << x << '\n'


uint32_t Deserialiser::activeFile = 0;


void Deserialiser::loadAssets(const rapidjson::Value& data)
{
	for (const auto& tex : data["textures"].GetArray())
		MNG_ASSERT_BASIC(p_scene->m_textures[tex[0].GetString()].loadFromFile(tex[1].GetString()));

	for (const auto& sha : data["shaders"].GetArray())
	{
		MNG_ASSERT_BASIC(p_scene->m_shaders[sha["name"].GetString()].loadFromFile(sha["path"].GetString(), (sf::Shader::Type)sha["type"].GetUint()));
		if (sha["texture"].IsTrue())
			p_scene->m_shaders[sha["name"].GetString()].setUniform("texture", sf::Shader::CurrentTexture);
	}
}

void Deserialiser::loadCamera(const rapidjson::Value& data)
{
	const auto& constr = data["constraints"];

	p_scene->m_camera.reset(sf::FloatRect(JsonToVec2<float>(data["pos"]), JsonToVec2<float>(data["size"])));
	p_scene->m_camera.setConstraints(JsonToVec2<float>(constr[0]), JsonToVec2<float>(constr[1]));
	p_scene->m_camera.enableConstraints(data["constrain"].IsTrue());
	p_scene->m_camera.setStaticTarget(JsonToVec2<float>(data["static-target"]));
	p_scene->m_camera.setDamping(5.f);
}

void Deserialiser::loadGraphics(const rapidjson::Value& data)
{
	for (const auto& spr : data.GetArray())
	{
		Sprite* s = nullptr;
		const char* layer = spr["layer"].GetString();
		if (strncmp(layer, "bg", 8) == 0)
			s = &p_scene->m_backgroundSprites.emplace_back();
		if (strncmp(layer, "fg", 8) == 0)
			s = &p_scene->m_foregroundSprites.emplace_back();
		MNG_ASSERT_SLIM(s);

		s->setPosition(JsonToVec2<float>(spr["pos"]));
		MNG_ASSERT_SLIM(p_scene->m_textures.find(spr["texture"].GetString()) != p_scene->m_textures.end());
		s->setTexture(p_scene->m_textures[spr["texture"].GetString()]);
		if (!spr["anim"].IsNull())
		{
			const auto& dims = spr["anim"]["dims"];
			s->animate(dims[0].GetUint(), dims[1].GetUint());
			s->setFrameRate(spr["anim"]["framerate"].GetFloat());
			s->setLoop(spr["anim"]["loop"].IsTrue());
			s->setIsPlaying(spr["anim"]["play"].IsTrue());
		}
		const auto& sha = spr["shader"];
		if (!sha.IsNull())
		{
			MNG_ASSERT_SLIM(p_scene->m_shaders.find(sha.GetString()) != p_scene->m_shaders.end());
			s->setShader(p_scene->m_shaders[sha.GetString()]);
		}
	}
}

void Deserialiser::loadPhysics(const rapidjson::Value& data)
{
	p_scene->m_physWorld = std::make_unique<b2World>(b2Vec2(0, 0));
	p_scene->m_physWorld->SetContactListener(&p_scene->m_listener);

	m_lockY = data["y-axis-locked"].IsTrue();

	m_scale = 1.f / data["scale"].GetFloat();
	p_scene->m_physScale = m_scale;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0, 0);
	b2Body* body = p_scene->m_physWorld->CreateBody(&bodyDef);
	MNG_ASSERT_SLIM(body);

	for (const auto& box : data["boxes"].GetArray())
	{
		const b2Vec2 points[4] = {
			JsonToB2Vec(box, m_scale),
			JsonToB2Vec(box, m_scale) + b2Vec2(box[2].GetFloat() * m_scale, 0.f),
			JsonToB2Vec(box, m_scale) + b2Vec2(box[2].GetFloat() * m_scale, box[3].GetFloat() * m_scale),
			JsonToB2Vec(box, m_scale) + b2Vec2(0.f, box[3].GetFloat() * m_scale),
		};
		b2PolygonShape shape;
		shape.Set(points, 4);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = &shape;
		fixtureDef.filter.categoryBits = 0x0001;
		fixtureDef.filter.maskBits = 0x0003;

		body->CreateFixture(&fixtureDef);

#ifdef MNG_DEBUG
		sf::RectangleShape debugShape;
		debugShape.setFillColor(sf::Color(0, 0, 0, 0));
		debugShape.setOutlineColor(sf::Color(255, 255, 255));
		debugShape.setOutlineThickness(0.3f);
		debugShape.setPosition(JsonToVec2<float>(box));
		debugShape.setSize(sf::Vector2f(box[2].GetFloat(), box[3].GetFloat()));
		p_scene->d_debugBoxColliders.push_back(debugShape);
#endif
	}

	for (const auto& chain : data["chains"].GetArray())
	{
		std::vector<b2Vec2> vertices;
		const auto& vdata = chain["verts"];
		vertices.resize(vdata.Size());
		for (size_t i = 0; i < vdata.Size(); i++)
		{
			vertices[i] = JsonToB2Vec(vdata[(rapidjson::SizeType)i], m_scale);

#ifdef MNG_DEBUG
			p_scene->d_debugChainColliders.append(JsonToVec2<float>(vdata[(rapidjson::SizeType)i]));
#endif
		}

		b2ChainShape shape;
		if (chain["loop"].IsTrue())
			shape.CreateLoop(vertices.data(), (uint32_t)vertices.size());
		else
		{
			const auto& ghosts = chain["ghosts"];
			shape.CreateChain(vertices.data(), (uint32_t)vertices.size(),
				JsonToB2Vec(ghosts[0], m_scale), JsonToB2Vec(ghosts[1], m_scale));
		}

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = &shape;
		fixtureDef.filter.categoryBits = 0x0001;
		fixtureDef.filter.maskBits = 0x0003;

		body->CreateFixture(&fixtureDef);
	}


	for (const auto& bt : data["box-triggers"].GetArray())
	{
		TriggerState* trig = &p_scene->m_triggers[bt["name"].GetString()];

		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0, 0);

		const auto& box = bt["rect"];

		const b2Vec2 points[4] = {
			JsonToB2Vec(box, m_scale),
			JsonToB2Vec(box, m_scale) + b2Vec2(box[2].GetFloat() * m_scale, 0.f),
			JsonToB2Vec(box, m_scale) + b2Vec2(box[2].GetFloat() * m_scale, box[3].GetFloat() * m_scale),
			JsonToB2Vec(box, m_scale) + b2Vec2(0.f, box[3].GetFloat() * m_scale),
		};
		b2PolygonShape shape;
		shape.Set(points, 4);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = &shape;
		fixtureDef.filter.categoryBits = 0x0002;
		fixtureDef.filter.maskBits = 0x0003;
		fixtureDef.isSensor = true;
		fixtureDef.userData.pointer = (uintptr_t)trig;

		b2Body* _body = p_scene->m_physWorld->CreateBody(&bodyDef);
		MNG_ASSERT_SLIM(_body);
		_body->CreateFixture(&fixtureDef);

#ifdef MNG_DEBUG
		sf::RectangleShape debugShape;
		debugShape.setFillColor(sf::Color(0, 0, 0, 0));
		debugShape.setOutlineColor(sf::Color(255, 255, 255));
		debugShape.setOutlineThickness(0.3f);
		debugShape.setPosition(JsonToVec2<float>(bt["rect"]));
		debugShape.setSize(sf::Vector2f(bt["rect"][2].GetFloat(), bt["rect"][3].GetFloat()));
		p_scene->d_debugBoxColliders.push_back(debugShape);
#endif
	}

	for (const auto& lt : data["line-triggers"].GetArray())
	{
		TriggerState* trig = &p_scene->m_triggers[lt["name"].GetString()];

		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0, 0);

		b2EdgeShape shape;
		shape.SetTwoSided(JsonToB2Vec(lt["start"], m_scale), JsonToB2Vec(lt["end"], m_scale));

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = &shape;
		fixtureDef.filter.categoryBits = 0x0002;
		fixtureDef.filter.maskBits = 0x0003;
		fixtureDef.isSensor = true;
		fixtureDef.userData.pointer = (uintptr_t)trig;

		b2Body* _body = p_scene->m_physWorld->CreateBody(&bodyDef);
		MNG_ASSERT_SLIM(_body);
		_body->CreateFixture(&fixtureDef);

#ifdef MNG_DEBUG
		sf::RectangleShape debugShape;
		debugShape.setFillColor(sf::Color(0, 0, 0, 0));
		debugShape.setOutlineColor(sf::Color(255, 255, 255));
		debugShape.setOutlineThickness(0.3f);
		debugShape.setPosition(JsonToVec2<float>(lt["start"]));
		debugShape.setSize(JsonToVec2<float>(lt["end"]) - JsonToVec2<float>(lt["start"]));
		p_scene->d_debugBoxColliders.push_back(debugShape);
#endif
	}
}

void Deserialiser::loadSounds(const rapidjson::Value& data)
{
	const auto& track = data["track"];
	if (track.IsNull())
	{
		AudioManager::track.stop();
		AudioManager::trackTitle = "";
	}
	else if (AudioManager::trackTitle != track["title"].GetString() || track["force-reset"].IsTrue())
	{
		MNG_ASSERT_BASIC(AudioManager::track.openFromFile(track["file"].GetString()));
		AudioManager::track.setVolume(AudioManager::getMusicVolume());
		AudioManager::track.setLoop(true);
		AudioManager::track.play();
		AudioManager::trackTitle = track["title"].GetString();
	}

	for (const auto& e : data["effects"].GetArray())
	{
		sf::Music& effect = AudioManager::addEffect(e["name"].GetString(), e["file"].GetString(), e["force-reset"].IsTrue());
		effect.setLoop(true);

		if (e["rand-start"].IsTrue())
		{
			const int offsetms = rand() % effect.getDuration().asMilliseconds();
			effect.setPlayingOffset(sf::milliseconds(offsetms));
		}

		effect.play();
	}

	AudioManager::clearNonPersisting();
}

void Deserialiser::loadEntities(const rapidjson::Value& data)
{
	for (const auto& e : data.GetArray())
	{
		std::unique_ptr<Entity> entity = nullptr;

		const char* type = e["type"].GetString();
		if (strncmp(type, "player", 32) == 0)
		{
			MNG_ASSERT_SLIM(!p_scene->m_player);
			entity = std::make_unique<Player>();
			entity->lock(e["locked"].IsTrue());
			p_scene->m_player = dynamic_cast<Player*>(entity.get());
		}
		else if (strncmp(type, "character", 32) == 0)
			entity = std::make_unique<Entity>();
		else if (strncmp(type, "static", 32) == 0)
			entity = std::make_unique<Entity>();

		MNG_ASSERT_SLIM(entity);

		entity->m_physScale = m_scale;
		entity->m_invScale = 1.f / m_scale;

		entity->set1D(m_lockY);
		if (e["pos"].IsNull())
			entity->setPosition(Scene::s_spawnpoint);
		else
			entity->setPosition(JsonToVec2<float>(e["pos"]));
		entity->setSize(JsonToVec2<float>(e["size"]));

		MNG_ASSERT_SLIM(p_scene->m_textures.find(e["texture"].GetString()) != p_scene->m_textures.end());

		const auto& anim = e["anim"];
		if (anim.IsNull())
			entity->m_sprite.setTexture(p_scene->m_textures[e["texture"].GetString()]);
		else
		{
			entity->m_sprite.setTexture(p_scene->m_textures[e["texture"].GetString()]);
			entity->m_sprite.animate(anim["frames"][0].GetUint(), anim["frames"][1].GetUint());
			entity->m_sprite.setFrameRate(anim["framerate"].GetFloat());
			entity->m_sprite.setLoop(true);
			entity->m_sprite.setIsPlaying(true);
		}

		if (e["camera-target"].IsTrue())
			p_scene->m_camera.setDynamicTarget(&entity->getPosition());

		const auto& phys = e["physics"];
		if (!phys.IsNull())
		{
			b2BodyDef bodydef;
			const sf::Vector2f physPos = (entity->getPosition() + JsonToVec2<float>(phys["offset"])) * m_scale;
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

			b2Body* body = p_scene->m_physWorld->CreateBody(&bodydef);
			MNG_ASSERT_SLIM(body);

			b2FixtureDef fixtureDef;
			fixtureDef.density = phys["density"].GetFloat();
			fixtureDef.filter.categoryBits = 0x0003;
			fixtureDef.filter.maskBits = 0x0001 | 0x0002;

			const auto& shape = phys["shape"];
			const char* stype = shape["type"].GetString();
			if (strncmp(stype, "box", 32) == 0)
			{
				b2PolygonShape s;
				s.SetAsBox(shape["size"][0].GetFloat() * m_scale * 0.5f, shape["size"][1].GetFloat() * m_scale * 0.5f);
				fixtureDef.shape = &s;
				body->CreateFixture(&fixtureDef);

				entity->simulate(body, JsonToVec2<float>(phys["offset"]), JsonToVec2<float>(shape["size"]));
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

		if (!e["script-handle"].IsNull())
			m_entityHandles.push_back(e["script-handle"].GetString());
		else
			m_entityHandles.push_back("0");
		p_scene->m_entities.push_back(std::move(entity));
	}
}

void Deserialiser::loadParticles(const rapidjson::Value& data)
{
	for (const auto& psd : data.GetArray())
	{
		sf::FloatRect bounds = JsonToRect(psd["bounds"]);
		ParticleSystem& ps = p_scene->m_particles.emplace_back(bounds);

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
			MNG_ASSERT_SLIM(p_scene->m_textures.find(psd["texture"].GetString()) != p_scene->m_textures.end());
			ps.setTexture(p_scene->m_textures[psd["texture"].GetString()]);
		}
	}
}

void Deserialiser::loadDialogue(const rapidjson::Value& data)
{
	if (!data.IsNull())
	{
		rapidjson::Document doc;
		loadjson(doc, std::string("res/dialogue/eng/") + data.GetString());

		for (const auto& p : doc.GetArray())
		{
			auto& que = p_scene->m_dialogue.m_alltext.emplace_back();

			for (const auto& str : p.GetArray())
				que.emplace_back(str.GetString());
		}

		MNG_ASSERT_BASIC(p_scene->m_fonts["pixel"].loadFromFile("res/fonts/equipmentpro.ttf"));
		p_scene->m_dialogue.m_text.setFont(p_scene->m_fonts["pixel"]);

		MNG_ASSERT_BASIC(p_scene->m_textures["dialogue"].loadFromFile("res/textures/gui/dialogue.png"));
		p_scene->m_dialogue.m_background.setTexture(p_scene->m_textures["dialogue"]);

		p_scene->m_dialogue.setPlaySpeed(40.f);
	}
}

void Deserialiser::loadTasks(const rapidjson::Value& data)
{
	if (!data.IsNull())
	{
		/*
		rapidjson::Document doc;
		loadjson(doc, std::string("res/dialogue/eng/") + data.GetString());

		for (const auto& p : doc.GetArray())
		{
			auto& que = p_scene->m_dialogue.m_alltext.emplace_back();

			for (const auto& str : p.GetArray())
				que.emplace_back(str.GetString());
		}
		*/
	}

	MNG_ASSERT_BASIC(p_scene->m_fonts["hand"].loadFromFile("res/fonts/cambria.ttf"));
	p_scene->m_agenda.p_font = &p_scene->m_fonts["hand"];

	MNG_ASSERT_BASIC(p_scene->m_textures["alert"].loadFromFile("res/textures/gui/alert.png"));
	p_scene->m_agenda.p_texture = &p_scene->m_textures["alert"];
}

void Deserialiser::loadScripts(const rapidjson::Value& data)
{
	p_scene->m_scripts.resize(data.Size());
	for (rapidjson::SizeType i = 0; i < data.Size(); i++)
	{
		Script::compile(data[i]["raw"].GetString(), m_entityHandles);
		p_scene->m_scripts[i].load(data[i]["bin"].GetString(), p_scene);

		for (const auto& t : data[i]["trigger"].GetArray())
			p_scene->m_triggers[t.GetString()].onEnter.bind(&Script::play, &p_scene->m_scripts[i]);
	}
}


Deserialiser::Deserialiser(Scene* scene) : p_scene(scene)
{

}


#define LOAD_MODULE(name, func) \
	loadjson(doc, "config/states/" name "/" + sname + ".json"); \
	if (doc[m_state].IsUint()) \
		func(doc[doc[m_state].GetUint()]); \
	else \
		func(doc[m_state])


void Deserialiser::run()
{
	std::string sname = p_scene->m_name;

	rapidjson::Document doc;
	loadjson(doc, "config/state" + std::to_string(activeFile) + ".json");

	m_state = doc[sname.c_str()].GetUint();

	p_scene->m_state = m_state;

	LOAD_MODULE("assets", loadAssets);
	LOAD_MODULE("camera", loadCamera);
	LOAD_MODULE("graphics", loadGraphics);
	LOAD_MODULE("physics", loadPhysics);
	LOAD_MODULE("audio", loadSounds);
	LOAD_MODULE("entities", loadEntities);
	//LOAD_MODULE("tasks", loadTasks);
	loadTasks({});

	loadjson(doc, "config/states/misc/" + sname + ".json");

	if (doc[m_state].IsUint())
	{
		loadParticles(doc[doc[m_state].GetUint()]["particles"]);
		loadScripts(doc[doc[m_state].GetUint()]["scripts"]);
		loadDialogue(doc[doc[m_state].GetUint()]["dialogue"]);
	}
	else
	{
		loadParticles(doc[m_state]["particles"]);
		loadScripts(doc[m_state]["scripts"]);
		loadDialogue(doc[m_state]["dialogue"]);
	}
}