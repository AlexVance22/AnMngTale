#pragma once

class Entity;
class Camera;
class Player;


class Script
{
private:
	enum class Op { WAIT = 0, PAUSE, OUT, PLACE, MOVE, STOP, LOCK, TRACK, VIEW, SPEAK, ANIM, PARTICLE };
	static const std::unordered_map<std::string, Op> s_opMap;
	static std::vector<Entity*> s_entities;
	static std::vector<std::string> s_handles;
	Camera* p_camera = nullptr;

	std::unordered_map<uint32_t, sf::Vector2f> m_directions;

	float m_delay = 0.f;
	bool m_running = false;
	bool m_end = false;
	int32_t m_output = -1;

	std::ifstream m_stream;

private:
	static void writeToken(std::ofstream& out, char* token);

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

public:
	static void pushEntity(Entity* entity_ptr, const std::string& handle);
	static void compile(const std::string& filepath);
	static void reset();

	static std::string suffix(const std::string& in);

public:
	Script() = default;
	Script(const std::string& filepath, Camera* camera);

	void load(const std::string& filepath, Camera* camera);

	void pause() { m_running = false; }
	void resume() { m_running = true; }
	bool isEnd() { return m_end; }

	int32_t output() { return m_output; }

	void update(const float deltaTime);
};