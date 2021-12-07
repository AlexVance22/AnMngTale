#pragma once

class Entity;
class Camera;
class Player;
class Scene;


class Script
{
private:
	enum class Op { WAIT = 0, PAUSE, OUT, PLACE, MOVE, STOP, LOCK, TRACK, VIEW, SPEAK, ANIM, PARTICLE, CAMVEL };
	static const std::unordered_map<std::string, Op> s_opMap;
	std::vector<Entity*> m_entities;
	Scene* p_scene = nullptr;

	std::unordered_map<uint32_t, sf::Vector2f> m_directions;

	float m_delay = 0.f;
	bool m_running = false;
	bool m_end = false;
	int32_t m_output = -1;

	std::ifstream m_stream;

private:
	static void writeToken(std::ofstream& out, char* token, const std::vector<std::string>& handles);

	void opInv();
	void opWait();
	void opPause();
	void opOut();
	void opPlace();
	void opMove();
	void opStop();
	void opLock();
	void opTrack();
	void opView();
	void opSpeak();
	void opAnim();
	void opParticle();
	void opCamvel();

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