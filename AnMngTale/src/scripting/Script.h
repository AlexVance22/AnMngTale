#pragma once

class Entity;
class Camera;
class Player;


class Script
{
private:
	enum class Op { WAIT = 0, PAUSE, OUT, PLACE, MOVE, STOP, LOCK, TRACK, VIEW, SPEAK, ANIM };
	static const std::unordered_map<std::string, Op> s_opMap;

	std::vector<std::unique_ptr<Entity>>* p_entities = nullptr;
	Camera* p_camera = nullptr;
	Player* p_player = nullptr;

	std::unordered_map<uint32_t, sf::Vector2f> m_directions;

	float m_delay = 0.f;
	bool m_running = true;
	bool m_end = false;
	int32_t m_output = -1;

	std::ifstream m_stream;

private:
	static void writeToken(std::ofstream& out, char* token);

	void opInv();
	void opWait();
	void opPause();
	void opEnd();
	void opOut();
	void opPlace();
	void opMove();
	void opStop();
	void opLock();
	void opTrack();
	void opView();
	void opSpeak();
	void opAnim();

public:
	Script() = default;
	Script(const std::string& filepath, std::vector<std::unique_ptr<Entity>>* entities, Camera* camera, Player* player = nullptr);

	void load(const std::string& filepath, std::vector<std::unique_ptr<Entity>>* entities, Camera* camera, Player* player = nullptr);

	static void compile(const std::string& filepath);

	void pause() { m_running = false; }
	void resume() { m_running = true; }
	bool isEnd() { return m_end; }

	int32_t output() { return m_output; }

	void update(const float deltaTime);
};