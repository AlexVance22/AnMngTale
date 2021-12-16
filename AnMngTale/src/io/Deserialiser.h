#pragma once

class Scene;


class Deserialiser
{
private:
	Scene* p_scene;
	uint32_t m_state = 0;
	std::vector<std::string> m_entityHandles;

	float m_scale = 1.f;
	bool m_lockY = false;

private:
	void loadAssets(const rapidjson::Value& data);
	void loadCamera(const rapidjson::Value& data);
	void loadGraphics(const rapidjson::Value& data);
	void loadPhysics(const rapidjson::Value& data);
	void loadSounds(const rapidjson::Value& data);
	void loadEntities(const rapidjson::Value& data);
	void loadParticles(const rapidjson::Value& data);
	void loadDialogue(const rapidjson::Value& data);
	void loadTasks(const rapidjson::Value& data);
	void loadScripts(const rapidjson::Value& data);

public:
	static uint32_t activeFile;

public:
	Deserialiser(Scene* scene);

	void run();
};