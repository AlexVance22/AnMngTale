#pragma once

#include "gui/widgets/containers/Container.h"


class MenuStack;
class Scene;


class Menu
{
private:
	TextureMap m_textures;
	FontMap m_fonts;

	gui::Root::Ptr m_widgets;

	bool m_quitTop = false;
	bool m_quitAll = false;
	bool m_hasNextScene = false;
	std::future<Obj<Scene>> m_nextScene;

	bool m_blocking = true;
	bool m_blurred = false;
	std::unordered_set<sf::Keyboard::Key> m_quitKeys;

	MenuStack* p_container = nullptr;

public:
	Menu();
	Menu(const std::string& filepath, MenuStack* stack);

	void loadFromFile(const std::string& filepath, MenuStack* stack);

	template<typename W>
	std::shared_ptr<W> getWidget(const std::string& name)
	{
		return gui::cast<W>(m_widgets->get(name));
	}

	template<typename S>
	void setNextScene()
	{
		m_nextScene = std::async(std::launch::async, []() -> Obj<Scene> { return std::make_unique<S>(); });
		m_hasNextScene = true;
	}
	bool hasNextScene() const;
	[[nodiscard]] std::future<Obj<Scene>>&& getNextScene();

	void scheduleQuit();
	bool getQuitTop();
	bool getQuitAll();

	bool isBlocking();
	bool isBlurred();
	
	void handleEvent(const sf::Event& event);
	void update(const float deltaTime);
	void render(sf::RenderTarget* target = nullptr);


	friend void pushAgenda(MenuStack* menus);
};