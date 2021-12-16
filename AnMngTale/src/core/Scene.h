#pragma once

#include "core/MenuStack.h"

#include "graphics/Camera.h"
#include "graphics/PostFX.h"
#include "graphics/ParticleSystem.h"

#include "entities/Entity.h"
#include "scripting/Script.h"
#include "physics/BoxTrigger.h"

#include "gui/DialogueHandler.h"


class Scene
{
public:
	static sf::RenderWindow* p_window;
	static sf::RenderTexture m_fadeBuffer;

protected:
	const std::string m_name;
	uint32_t m_state = 0;
	bool m_progress = false;

	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<std::string, sf::Shader> m_shaders;
	std::unordered_map<std::string, sf::Font> m_fonts;
	std::vector<Sprite> m_backgroundSprites;
	std::vector<Sprite> m_foregroundSprites;

	std::unordered_map<std::string, BoxTrigger> m_triggers;
	std::vector<std::unique_ptr<Entity>> m_entities;
	Player* m_player = nullptr;

	std::vector<ParticleSystem> m_particles;
	std::vector<Script> m_scripts;

	DialogueHandler m_dialogue;

	Obj<Scene> m_nextScene = nullptr;
	MenuStack m_overlays;
	bool m_quit = false;

	std::unique_ptr<b2World> m_physWorld;
	float m_physScale = 1.f;

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

public:
	Scene(const std::string& scenename);
	~Scene();

	bool quit() const;
	[[nodiscard]] Obj<Scene>&& nextScene();

	void fadeout();
	void fadein();

	template<typename S, typename std::enable_if_t<std::is_base_of<Scene, S>::value>* = nullptr>
	void loadScene()
	{
		std::future<Obj<Scene>> next = std::async(std::launch::async, []() -> Obj<Scene> { return std::make_unique<S>(); });
		m_quit = true;
		fadeout();
		m_nextScene = std::move(next.get());
	}

	virtual void update(const float deltaTime);
	virtual void render(sf::RenderTarget* target = nullptr);


	friend class Deserialiser;
	friend class Script;
};