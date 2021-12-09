#pragma once

#include "Scene.h"


class Game
{
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;

	Obj<Scene> m_scene;

private:
	void loadSession();
	void dumpSession();

	void update();
	void render();

public:
	Game();
	~Game();

	void run();
};