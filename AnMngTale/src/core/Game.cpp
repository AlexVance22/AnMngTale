#include "PCH.h"
#include "Game.h"

#include "scenes/MainMenu.h"
#include "scenes/outside/Courtyard.h"
#include "scenes/floors/Floor300.h"

#include "global/Data.h"

#include "scripting/Script.h"

#define OVERRIDE

#ifdef MNG_DEBUG
	#define SCREENSPACE sf::Style::Default
#endif
#ifdef MNG_RELEASE
	#ifndef OVERRIDE
		#define SCREENSPACE sf::Style::Fullscreen
	#else
		#define SCREENSPACE sf::Style::Default
	#endif
#endif
#ifdef MNG_DIST
	#define SCREENSPACE sf::Style::Fullscreen
#endif


void Game::update()
{
	const float deltaTime = m_clock.restart().asSeconds();

	m_scene->update(deltaTime);

	if (m_scene->quit())
	{
		if (m_scene->nextScene())
		{
			Scene* const buffer = m_scene->nextScene();
			delete m_scene;
			m_scene = buffer;
		}
		else
		{
			delete m_scene;
			m_scene = nullptr;
			m_window.close();
		}
	}
}

void Game::render()
{
	if (m_scene)
	{
		m_scene->render();
		m_window.display();
	}
}


Game::Game() : m_window(sf::VideoMode(1920, 1080), "AnMngTale", SCREENSPACE)
{
	m_window.setVerticalSyncEnabled(true);
	Scene::p_window = &m_window;
	m_scene = new Courtyard();

	Script::compile("res/scripts/courtyard1.txt");
}


void Game::run()
{
	while (m_window.isOpen())
	{
		update();
		render();
	}
}