#pragma once

#include "core/Menu.h"

#include "graphics/Camera.h"
#include "graphics/PostFX.h"

#include "entities/Entity.h"
#include "physics/BoxTrigger.h"

#include "graphics/ParticleSystem.h"

#include "scripting/Script.h"


const char* freadall(const std::string& filepath);

void loadjson(rapidjson::Document& doc, const std::string& filepath);

void dumpjson(const rapidjson::Document& doc, const std::string& filepath);


class Scene
{
public:
	static sf::RenderWindow* p_window;

	struct MatSprite {
		Sprite sprite;
		sf::RenderStates states;
	};

protected:
	enum class AreaState { INTRO, DEFAULT, EXPO, BUILD, CLIMAX };
	AreaState m_state;
	const std::string m_name;

	//static const std::unordered_map<AreaState, const char*> stateToStr;

	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<std::string, sf::Shader> m_shaders;
	std::vector<MatSprite> m_backgroundSprites;
	std::vector<MatSprite> m_foregroundSprites;

	std::unordered_map<std::string, BoxTrigger> m_triggers;
	std::vector<std::unique_ptr<Entity>> m_entities;
	Player* m_player = nullptr;

	std::vector<ParticleSystem> m_particles;
	std::vector<Script> m_scripts;

	std::stack<Menu> m_overlays;
	Scene* m_nextScene = nullptr;
	bool m_quit = false;

	std::unique_ptr<b2World> m_physWorld;
	const static float s_physScale;

	Camera m_camera;
	PostFX m_postfx;

#ifdef _DEBUG
	sf::VertexArray d_debugChainColliders;
	std::vector<sf::RectangleShape> d_debugBoxColliders;

	sf::Font d_debugFont;
	sf::Text d_framerate, d_tUpdate, d_tRender;
	sf::Text d_tPhysics, d_tUserlogic;
#endif

	static sf::RenderTexture m_fadeTarget;

protected:
	void handleGui(const float deltaTime);

	//void loadFlags(const rapidjson::Value& data);
	void loadCamera(const rapidjson::Value& data);
	void loadGraphics(const rapidjson::Value& data);
	void loadPhysics(const rapidjson::Value& data);
	void loadSounds(const rapidjson::Value& data);
	void loadEntities(const rapidjson::Value& data);
	void loadParticles(const rapidjson::Value& data);
	void loadScripts(const rapidjson::Value& data);

	void reloadResources(bool clear = false);

	void baseEvents(const sf::Event& event);
	virtual void handleEvents() = 0;
	virtual void impl(const float deltaTime) = 0;

public:
	Scene(const std::string& scenename);
	virtual ~Scene() = default;

	bool quit() const { return m_quit; }
	Scene* nextScene() { return m_nextScene; }

	template<typename S>
	void loadScene()
	{
		m_postfx.setEnabled("blur", true);
		render();
		m_postfx.setEnabled("blur", false);

		p_window->draw(sf::Sprite(m_textures["loadingscreen"]));
		p_window->display();

		m_nextScene = new S();
		m_quit = true;
	}

	virtual void update(const float deltaTime);
	virtual void render(sf::RenderTarget* target = nullptr);
};