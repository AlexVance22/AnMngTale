#pragma once

#include "core/Menu.h"

#include "graphics/Camera.h"
#include "graphics/PostFX.h"

#include "entities/Entity.h"
#include "physics/BoxTrigger.h"

#include "scripting/Script.h"

#include "graphics/ParticleSystem.h"


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

	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<std::string, sf::Shader> m_shaders;
	std::vector<MatSprite> m_backgroundSprites;
	std::vector<MatSprite> m_foregroundSprites;

	std::unordered_map<std::string, BoxTrigger> m_triggers;
	std::vector<std::unique_ptr<Entity>> m_entities;
	Player* m_player = nullptr;

	std::vector<std::vector<std::string>> m_dialogue;
	std::vector<ParticleSystem> m_particles;
	std::vector<Script> m_scripts;

	Obj<Scene> m_nextScene = nullptr;
	std::stack<Menu> m_overlays;
	bool m_quit = false;

	std::unique_ptr<b2World> m_physWorld;
	const static float s_physScale;

	static sf::RenderTexture m_fadeBuffer;

	Camera m_camera;
	PostFX m_postfx;

#ifdef _DEBUG
	sf::VertexArray d_debugChainColliders;
	std::vector<sf::RectangleShape> d_debugBoxColliders;

	sf::Font d_debugFont;
	sf::Text d_framerate, d_tUpdate, d_tRender;
	sf::Text d_tPhysics, d_tUserlogic;
#endif

protected:
	void handleGui(const float deltaTime);
	void handleGame(const float deltaTime);

	void handleEvents();
	void handleEventDefault(const sf::Event& event);
	virtual void handleEventSpecial(const sf::Event& event);
	virtual void impl(const float deltaTime) = 0;

private:
	void reloadResources(bool clear = false);
	void pushMenu(const std::string& filepath);

public:
	Scene(const std::string& scenename);
	virtual ~Scene() = default;

	bool quit() const;
	Obj<Scene>&& nextScene();

	template<typename S, typename std::enable_if_t<std::is_base_of<Scene, S>::value>* = nullptr>
	void loadScene()
	{
		m_postfx.setEnabled("blur", true);
		render();
		m_postfx.setEnabled("blur", false);

		p_window->draw(sf::Sprite(m_textures["loadingscreen"]));
		p_window->display();

		m_nextScene = std::make_unique<S>();
		m_quit = true;
	}

	virtual void update(const float deltaTime);
	virtual void render(sf::RenderTarget* target = nullptr);


	friend class Deserialiser;
	friend class Script;
};