#include "PCH.h"
#include "Script.h"

#include "core/Scene.h"
#include "core/Asserts.h"

#include "entities/Entity.h"
#include "entities/Player.h"
#include "graphics/Camera.h"

#include "gui/callbacks/CBCore.h"

#ifdef MNG_DEBUG
	#define RECOMPILE
#endif


const char* freadall(const std::string& filepath);

template<typename Ty>
void fread(std::ifstream& stream, Ty& ptr)
{
	stream.read((char*)&ptr, sizeof(Ty));
}

static int isnumerical(char* str)
{
	bool decimal = false;

	if (!std::isdigit(*str) && *str != '-')
		return -1;

	for (char* c = str + 1; *c != '\0'; c++)
	{
		if (!std::isdigit(*c))
		{
			if (*c == '.' && !decimal)
				decimal = true;
			else
				return -1;
		}
	}

	return decimal;
}


const std::unordered_map<std::string, Script::Op> Script::s_opMap{
	{ "wait", Op::WAIT },
	{ "pause", Op::PAUSE },
	{ "out", Op::OUT },
	{ "place", Op::PLACE },
	{ "move", Op::MOVE },
	{ "curve", Op::CURVE },
	{ "stop", Op::STOP },
	{ "lock", Op::LOCK },
	{ "track", Op::TRACK },
	{ "view", Op::VIEW },
	{ "camvel", Op::CAMVEL },
	{ "speak", Op::SPEAK },
	{ "anim", Op::ANIM },
	{ "particle", Op::PARTICLE },
	{ "assign", Op::ASSIGN }
};


void Script::opInv()
{
	MNG_ASSERT_MSG(false, "Script runtime invalid operation");
}
void Script::opWait()
{
	fread(m_stream, m_delay);
}
void Script::opPause()
{
	fread(m_stream, m_output);

	m_running = false;

	for (auto& [_, dir] : m_directions)
		dir = sf::Vector2f(0, 0);
}
void Script::opOut()
{
	fread(m_stream, m_output);
}
void Script::opPlace()
{
	uint32_t entity;
	fread(m_stream, entity);
	sf::Vector2f pos;
	fread(m_stream, pos);
	m_entities[entity]->setPosition(pos);
}
void Script::opMove()
{
	uint32_t entity;
	fread(m_stream, entity);
	sf::Vector2f dir;
	fread(m_stream, dir);
	sf::Vector2f normal = dir / (std::sqrt(dir.x * dir.x + dir.y * dir.y));
	m_directions[entity] = normal;
}
void Script::opCurve()
{
	uint32_t entity;
	fread(m_stream, entity);
	float angle;
	fread(m_stream, angle);
	float speed;
	fread(m_stream, speed);

	m_curves[entity] = { angle, speed };

	bool autotime;
	fread(m_stream, autotime);
	if (autotime)
		m_delay = std::abs(angle) / (180.f * speed);
}
void Script::opStop()
{
	uint32_t entity;
	fread(m_stream, entity);
	m_directions[entity] = sf::Vector2f(0, 0);
}
void Script::opLock()
{
	uint32_t entity;
	fread(m_stream, entity);
	bool lock;
	fread(m_stream, lock);
	m_entities[entity]->lock(lock);
}
void Script::opTrack()
{
	uint32_t entity;
	fread(m_stream, entity);
	p_scene->m_camera.setDynamicTarget(&m_entities[entity]->getPosition());
}
void Script::opView()
{
	sf::Vector2f pos;
	fread(m_stream, pos);
	p_scene->m_camera.setStaticTarget(pos);
}
void Script::opCamvel()
{
	float vel;
	fread(m_stream, vel);

	p_scene->m_camera.setDamping(vel);
}
void Script::opSpeak()
{
	uint32_t page;
	fread(m_stream, page);
	p_scene->m_dialogue.begin(page);
}
void Script::opAnim()
{
	uint32_t entity;
	fread(m_stream, entity);
	uint32_t anim;
	fread(m_stream, anim);
	m_entities[entity]->m_sprite.setAnimation(anim);
}
void Script::opParticle()
{

}
void Script::opAssign()
{

}


void Script::computeCurves(const float deltaTime)
{
	constexpr float radconvert = 3.14159f / 180.f;

	std::vector<uint32_t> to_erase;

	for (const auto [e, curve] : m_curves)
	{
		const float step = curve.speed * 3.f * ((curve.angle < 0.f) - (curve.angle > 0.f));
		const float radstep = step * radconvert;
		const float cosstep = std::cos(radstep);
		const float sinstep = std::sin(radstep);

		const sf::Vector2f base = m_directions[e];

		m_directions[e].x = base.x * cosstep - base.y * sinstep;
		m_directions[e].y = base.x * sinstep + base.y * cosstep;

		m_curves[e].angle += step * deltaTime * 60.f;

		if (std::abs(curve.angle) < 1.6f)
			to_erase.push_back(e);
	}

	for (const uint32_t e : to_erase)
		m_curves.erase(e);
}


void Script::writeToken(std::ofstream& out, char* token, const std::vector<std::string>& handles)
{
	switch (isnumerical(token))
	{
	case 0: {
		const uint32_t num = std::atoi(token);
		out.write((char*)&num, sizeof(uint32_t));
		memset(token, 0, 32);
		return; }
	case 1: {
		const float num = (float)std::atof(token);
		out.write((char*)&num, sizeof(float));
		memset(token, 0, 32);
		return; }
	}
	if (strncmp(token, "false", 32) == 0)
	{
		const bool val = false;
		out.write((char*)&val, sizeof(bool));
		memset(token, 0, 32);
		return;
	}
	if (strncmp(token, "true", 32) == 0)
	{
		const bool val = true;
		out.write((char*)&val, sizeof(bool));
		memset(token, 0, 32);
		return;
	}

	const auto it1 = s_opMap.find(token);
#ifdef _DEBUG
	if (it1 == s_opMap.end())
	{
		const auto it2 = std::find_if(handles.begin(), handles.end(),
			[&](const std::string& handle) { return token == handle; });
		if (it2 == handles.end())
		{
			out.close();
			MNG_ASSERT_MSG(false, "Invalid compilation token");
		}

		uint32_t idx = (uint32_t)(it2 - handles.begin());
		out.write((char*)&idx, sizeof(uint32_t));
		memset(token, 0, 32);
		return;
	}
#endif
	char op = (char)it1->second;
	out.write(&op, sizeof(char));
	memset(token, 0, 32);
}

void Script::compile(const std::string& filepath, const std::vector<std::string>& handles)
{
#ifdef RECOMPILE
	const char* buf = freadall(filepath);
	std::ofstream out(filepath.substr(0, filepath.size() - 4) + ".mng");
	MNG_ASSERT_SLIM(out.is_open());

	size_t toki = 0;
	bool white = false;
	char token[32] = { '\0' };

	for (const char* c = buf; *c != '\0'; c++)
	{
		if (std::isspace(*c))
		{
			if (!white)
			{
				toki = 0;
				writeToken(out, token, handles);
				white = true;
			}
		}
		else
		{
			token[toki++] = *c;
			white = false;
		}
	}

	writeToken(out, token, handles);
	delete[] buf;
#endif
}


Script::Script(const std::string& filepath, Scene* scene)
{
	load(filepath, scene);
}

void Script::load(const std::string& filepath, Scene* scene)
{
	m_stream.open(filepath, std::ios::binary);
	MNG_ASSERT_SLIM(m_stream.is_open());

	MNG_ASSERT_SLIM(scene);
	p_scene = scene;

	for (auto& e : p_scene->m_entities)
		m_entities.push_back(e.get());

	for (auto [_, dir] : m_directions)
		dir = sf::Vector2f(0, 0);
}


void Script::update(const float deltaTime)
{
	if (m_running)
	{
		if (m_delay <= 0.f && !p_scene->m_dialogue.isPlaying())
		{
			m_output = -1;

			while (!m_end)
			{
				char opcode = -1;
				fread(m_stream, opcode);

				switch ((Op)opcode)
				{
				case Op::WAIT:
					opWait();
					return;
				case Op::PAUSE:
					opPause();
					return;
				case Op::OUT:
					opOut();
					return;
				case Op::PLACE:
					opPlace();
					break;
				case Op::MOVE:
					opMove();
					break;
				case Op::CURVE:
					opCurve();
					return;
				case Op::STOP:
					opStop();
					return;
				case Op::LOCK:
					opLock();
					break;
				case Op::TRACK:
					opTrack();
					break;
				case Op::VIEW:
					opView();
					break;
				case Op::CAMVEL:
					opCamvel();
					break;
				case Op::SPEAK:
					opSpeak();
					return;
				case Op::ANIM:
					opAnim();
					break;
				case Op::PARTICLE:
					opParticle();
					break;
				case Op::ASSIGN:
					opAssign();
					break;
				default:
					//opInv();
					break;
				}

				m_end = m_stream.eof();
			}

			m_running = false;
		}

		if (m_delay > 0.f)
		{
			m_delay -= deltaTime;

			computeCurves(deltaTime);

			for (size_t i = 0; i < m_entities.size(); i++)
			{
				if (m_directions[(uint32_t)i] != sf::Vector2f(0, 0))
					m_entities[i]->move(m_directions[(uint32_t)i], deltaTime, true);
			}
		}
	}
}