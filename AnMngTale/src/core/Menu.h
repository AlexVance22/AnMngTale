#pragma once

#include "gui/widgets/containers/Container.h"


//#define BEGIN_MENU(stack, window, filepath) stack.push(std::make_shared<Menu>(window, stack, filepath))
//#define PUSH_MENU(filepath) p_menus->push(std::make_shared<Menu>(p_window, p_menus, filepath))


class Scene;

class Menu
{
private:
	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<std::string, sf::Font> m_fonts;

	sf::RenderWindow* p_window = nullptr;
	std::stack<Menu>* p_menus;
	gui::Root::Ptr m_widgets;

	sf::View m_view;

	bool m_quit = false;
	bool m_masterQuit = false;
	Scene* m_nextScene = nullptr;

public:
	Menu(sf::RenderWindow* window, std::stack<Menu>* menus, const std::string& filepath);
	Menu(sf::RenderWindow* window, std::stack<Menu>* menus);

	void loadFromFile(const std::string& filepath);

	template<typename _Widget>
	std::shared_ptr<_Widget> getWidget(const std::string& name)
	{
		return gui::cast<_Widget>(m_widgets->get(name));
	}

	template<typename S>
	void setNextScene()
	{
		m_nextScene = new S();
	}
	void scheduleQuit() { m_quit = true; }

	bool getQuit() { return m_quit; }
	bool getMasterQuit() { return m_masterQuit; }
	Scene* getNextScene() { return m_nextScene; }
	
	void handleEvents();
	void update(const float deltaTime);
	void render(sf::RenderTarget* target = nullptr);
};