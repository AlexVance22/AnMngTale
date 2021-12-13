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
	sf::View m_view;

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

	template<typename _Widget, typename std::enable_if_t<std::is_base_of<gui::Widget, _Widget>::value>* = nullptr>
	std::shared_ptr<_Widget> getWidget(const std::string& name)
	{
		return gui::cast<_Widget>(m_widgets->get(name));
	}

	template<typename S, typename std::enable_if_t<std::is_base_of<Scene, S>::value>* = nullptr>
	void setNextScene()
	{
		m_nextScene = std::async(std::launch::async, []() -> Obj<Scene> { return std::make_unique<S>(); });
		m_hasNextScene = true;
	}
	bool hasNextScene() const;
	std::future<Obj<Scene>>&& getNextScene();

	void scheduleQuit();
	bool getQuitTop();
	bool getQuitAll();

	bool isBlocking();
	bool isBlurred();
	
	void handleEvent(const sf::Event& event);
	void update(const float deltaTime);
	void render(sf::RenderTarget* target = nullptr);
};