#pragma once

class Scene;


class Deserialiser
{
private:
	Scene* p_scene;

	std::vector<std::string> m_entityHandles;

private:
	void loadCamera(const rapidjson::Value& data);
	void loadGraphics(const rapidjson::Value& data);
	void loadPhysics(const rapidjson::Value& data);
	void loadSounds(const rapidjson::Value& data);
	void loadEntities(const rapidjson::Value& data);
	void loadParticles(const rapidjson::Value& data);
	void loadDialogue(const rapidjson::Value& data);
	void loadScripts(const rapidjson::Value& data);

public:
	Deserialiser(Scene* scene);

	void run();
};