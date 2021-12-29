#pragma once

class Entity;
class Scene;


class Script
{
private:
	enum class Op { WAIT = 0, PAUSE, OUT, PLACE, MOVE, CURVE, STOP, LOCK, TRACK, VIEW, CAMVEL, SPEAK, ANIM, PARTICLE, ASSIGN };
	static const std::unordered_map<std::string, Op> s_opMap;

	Scene* p_scene = nullptr;

	std::vector<Entity*> m_entities;
	std::unordered_map<uint32_t, sf::Vector2f> m_directions;

	struct Curve { float angle; float speed; };
	std::unordered_map<uint32_t, Curve> m_curves;

	float m_delay = 0.f;
	bool m_running = false;
	bool m_end = false;
	int32_t m_output = -1;

	std::ifstream m_stream;

private:
	void opInv();
	void opWait();
	void opPause();
	void opOut();
	void opPlace();
	void opMove();
	void opCurve();
	void opStop();
	void opLock();
	void opTrack();
	void opView();
	void opCamvel();
	void opSpeak();
	void opAnim();
	void opParticle();
	void opAssign();

	void computeCurves(const float deltaTime);

private:
	static void writeToken(std::ofstream& out, char* token, const std::vector<std::string>& handles);

public:
	static void compile(const std::string& filepath, const std::vector<std::string>& handles);

public:
	Script() = default;
	Script(const std::string& filepath, Scene* scene);

	void load(const std::string& filepath, Scene* scene);

	void play() { m_running = true; }
	void pause() { m_running = false; }
	bool isEnd() const { return m_end; }

	int32_t output() const { return m_output; }

	void update(const float deltaTime);
};