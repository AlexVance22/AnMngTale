#include "PCH.h"
#include "Deserialiser.h"

#include "JsonUtils.h"

#include "core/Scene.h"
#include "core/Asserts.h"
#include "entities/Player.h"
#include "global/AudioManager.h"


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
	for (const auto& tex : data["textures"].GetArray())
		MNG_ASSERT_BASIC(p_scene->m_textures[tex[0].GetString()].loadFromFile(tex[1].GetString()));
	for (const auto& sha : data["shaders"].GetArray())
	{
		MNG_ASSERT_BASIC(p_scene->m_shaders[sha["name"].GetString()].loadFromFile(sha["path"].GetString(), (sf::Shader::Type)sha["type"].GetUint()));
		if (sha["texture"].IsTrue())
			p_scene->m_shaders[sha["name"].GetString()].setUniform("texture", sf::Shader::CurrentTexture);
	}

	for (const auto& spr : data["sprites"].GetArray())
	{
		Scene::MatSprite* s = nullptr;
		const char* layer = spr["layer"].GetString();
		if (strncmp(layer, "bg", 8) == 0)
			s = &p_scene->m_backgroundSprites.emplace_back();
		if (strncmp(layer, "fg", 8) == 0)
			s = &p_scene->m_foregroundSprites.emplace_back();
		MNG_ASSERT_SLIM(s);

		s->sprite.setPosition(JsonToVec2<float>(spr["pos"]));
		MNG_ASSERT_SLIM(p_scene->m_textures.find(spr["texture"].GetString()) != p_scene->m_textures.end());
		s->sprite.setTexture(p_scene->m_textures[spr["texture"].GetString()]);
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
			MNG_ASSERT_SLIM(p_scene->m_shaders.find(sha.GetString()) != p_scene->m_shaders.end());
			s->states.shader = &p_scene->m_shaders[sha.GetString()];
		}
	}
}

void Deserialiser::loadPhysics(const rapidjson::Value& data)
{
	p_scene->m_physWorld = std::make_unique<b2World>(b2Vec2(0, 0));

	for (const auto& box : data["boxes"].GetArray())
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set((box[0].GetFloat() - box[2].GetFloat() * 0.5f) * Scene::s_physScale,
			(box[1].GetFloat() - box[3].GetFloat() * 0.5f) * Scene::s_physScale);

		b2PolygonShape shape;
		shape.SetAsBox(box[2].GetFloat() * 0.5f * Scene::s_physScale, box[3].GetFloat() * 0.5f * Scene::s_physScale);

#ifdef MNG_DEBUG
		sf::RectangleShape debugShape;
		debugShape.setFillColor(sf::Color(0, 0, 0, 0));
		debugShape.setOutlineColor(sf::Color(255, 255, 255));
		debugShape.setOutlineThickness(0.3f);
		debugShape.setPosition(box[0].GetFloat() - box[2].GetFloat(), box[1].GetFloat() - box[3].GetFloat());
		debugShape.setSize(sf::Vector2f(box[2].GetFloat(), box[3].GetFloat()));
		p_scene->d_debugBoxColliders.push_back(debugShape);
#endif

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = &shape;

		b2Body* body = p_scene->m_physWorld->CreateBody(&bodyDef);
		MNG_ASSERT_SLIM(body);
		body->CreateFixture(&fixtureDef);
	}

	for (const auto& chain : data["chains"].GetArray())
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0, 0);

		std::vector<b2Vec2> vertices;
		const auto& vdata = chain["verts"];
		vertices.resize(vdata.Size());
		for (size_t i = 0; i < vdata.Size(); i++)
		{
			vertices[i] = JsonToB2Vec(vdata[(rapidjson::SizeType)i], Scene::s_physScale);

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
				JsonToB2Vec(ghosts[0], Scene::s_physScale), JsonToB2Vec(ghosts[1], Scene::s_physScale));
		}

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = &shape;

		b2Body* body = p_scene->m_physWorld->CreateBody(&bodyDef);
		MNG_ASSERT_SLIM(body);
		body->CreateFixture(&fixtureDef);
	}

	for (const auto& bt : data["box-triggers"].GetArray())
	{
		p_scene->m_triggers[bt["name"].GetString()] = BoxTrigger(JsonToRect(bt["rect"]));

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
			const sf::Vector2f physPos = (entity->getPosition() + JsonToVec2<float>(phys["offset"])) * Scene::s_physScale;
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

			b2FixtureDef fixturedef;
			fixturedef.density = phys["density"].GetFloat();

			const auto& shape = phys["shape"];
			const char* stype = shape["type"].GetString();
			if (strncmp(stype, "box", 32) == 0)
			{
				b2PolygonShape s;
				s.SetAsBox(shape["size"][0].GetFloat() * Scene::s_physScale * 0.5f, shape["size"][1].GetFloat() * Scene::s_physScale * 0.5f);
				fixturedef.shape = &s;
				body->CreateFixture(&fixturedef);

				const sf::FloatRect collider(JsonToVec2<float>(phys["offset"]), JsonToVec2<float>(shape["size"]));

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
			auto& vec = p_scene->m_dialogueText.emplace_back();

			for (const auto& str : p.GetArray())
				vec.emplace_back(str.GetString());
		}

		MNG_ASSERT_BASIC(p_scene->m_fonts["pixel"].loadFromFile("res/fonts/equipmentpro.ttf"));
		p_scene->m_dialogue.m_text.setFont(p_scene->m_fonts["pixel"]);

		MNG_ASSERT_BASIC(p_scene->m_textures["dialogue"].loadFromFile("res/textures/gui/dialogue.png"));
		p_scene->m_dialogue.m_background.setTexture(p_scene->m_textures["dialogue"]);

		p_scene->m_dialogue.setPlaySpeed(40.f);
	}
}

void Deserialiser::loadScripts(const rapidjson::Value& data)
{
	p_scene->m_scripts.resize(data.Size());
	for (rapidjson::SizeType i = 0; i < data.Size(); i++)
	{
		Script::compile(data[i]["raw"].GetString(), m_entityHandles);
		p_scene->m_scripts[i].load(data[i]["bin"].GetString(), p_scene);

		for (const auto& t : data[i]["trigger"].GetArray())
			p_scene->m_triggers[t.GetString()].onCollide.bind(&Script::play, &p_scene->m_scripts[i]);
	}
}


Deserialiser::Deserialiser(Scene* scene) : p_scene(scene)
{

}


#define LOAD_MODULE(name, func) \
	loadjson(doc, "config/saves/" name "/" + sname + ".json"); \
	if (doc[m_state].IsUint()) \
		func(doc[doc[m_state].GetUint()]); \
	else \
		func(doc[m_state])


void Deserialiser::run()
{
	std::string sname = p_scene->m_name;

	rapidjson::Document doc;
	loadjson(doc, "config/saves/state0.json");

	m_state = doc[sname.c_str()].GetUint();

	p_scene->m_state = m_state;

	LOAD_MODULE("camera", loadCamera);
	LOAD_MODULE("graphics", loadGraphics);
	LOAD_MODULE("physics", loadPhysics);
	LOAD_MODULE("audio", loadSounds);
	LOAD_MODULE("entities", loadEntities);

	loadjson(doc, "config/saves/misc/" + sname + ".json");

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