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
	std::stack<Menu>* p_overlays = nullptr;
	gui::Root::Ptr m_widgets;

	sf::View m_view;

	bool m_quitTop = false;
	bool m_quitAll = false;
	Obj<Scene> m_nextScene = nullptr;

	bool m_blocking = true;
	bool m_blurred = false;
	bool m_quitOnEsc = true;

public:
	Menu(sf::RenderWindow* window, std::stack<Menu>* overlays, const std::string& filepath);
	Menu(sf::RenderWindow* window, std::stack<Menu>* overlays);

	void loadFromFile(const std::string& filepath);

	template<typename _Widget, typename std::enable_if_t<std::is_base_of<gui::Widget, _Widget>::value>* = nullptr>
	std::shared_ptr<_Widget> getWidget(const std::string& name)
	{
		return gui::cast<_Widget>(m_widgets->get(name));
	}

	template<typename S, typename std::enable_if_t<std::is_base_of<Scene, S>::value>* = nullptr>
	void setNextScene()
	{
		m_nextScene = std::make_unique<S>();
	}
	Obj<Scene>&& getNextScene();

	void scheduleQuit();
	bool getQuitTop();
	bool getQuitAll();

	bool isBlocking();
	bool isBlurred();
	
	void handleEvent(const sf::Event& event);
	void update(const float deltaTime);
	void render(sf::RenderTarget* target = nullptr);
};