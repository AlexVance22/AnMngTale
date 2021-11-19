#pragma once

#include "Scene.h"


class Game
{
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;

	Scene* m_scene;

	void update();
	void render();

public:
	Game();

	void run();
};