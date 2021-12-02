#pragma once

#include "Scene.h"


class Game
{
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;

	Obj<Scene> m_scene;

	void update();
	void render();

public:
	Game();

	void run();
};