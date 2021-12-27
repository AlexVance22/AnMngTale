#include "PCH.h"
#include "Game.h"

#include "Asserts.h"

#include "global/AudioManager.h"
#include "global/Data.h"

#include "scenes/MainMenu.h"
#include "scenes/floors/Floor300.h"
#include "scenes/outside/Courtyard.h"
#include "scenes/rooms/Classroom.h"


#ifndef MNG_DIST
	#define SCREENSPACE sf::Style::Default
#else
	#define SCREENSPACE sf::Style::Fullscreen
#endif


void Game::loadSession()
{
	std::ifstream stream("config/startup.ini");
	MNG_ASSERT_SLIM(stream.is_open());

	float gvol, mvol, evol;
	stream >> gvol >> mvol >> evol;

	AudioManager::setGlobalVolume(gvol);
	AudioManager::setMusicVolume(mvol);
	AudioManager::setEffectVolume(evol);

	stream >> Data::language;

	sf::VideoMode mode;
	stream >> mode.width >> mode.height;

	m_window.create(mode, "AnMngTale", SCREENSPACE);
	m_window.setVerticalSyncEnabled(true);

	sf::Image cursortex;
	cursortex.loadFromFile("res/textures/gui/cursor.png");
	m_cursor.loadFromPixels(cursortex.getPixelsPtr(), sf::Vector2u(16, 16), sf::Vector2u(0, 0));
	m_window.setMouseCursor(m_cursor);
}

void Game::dumpSession()
{
	std::ofstream stream("config/startup.ini");
	MNG_ASSERT_SLIM(stream.is_open());

	stream << AudioManager::getGlobalVolume() << '\n'
			<< AudioManager::getMusicVolume() << '\n'
			<< AudioManager::getEffectVolume() << '\n';

	stream << Data::language << '\n';

	stream << 1920 << '\n' << 1080;
}


void Game::update()
{
	const float deltaTime = m_clock.restart().asSeconds();

	m_scene->update(deltaTime);

	if (m_scene->quit())
	{
		if (m_scene->nextScene())
		{
			m_scene = m_scene->nextScene();
			m_scene->fadein();
		}
		else
			m_window.close();
	}
}

void Game::render()
{
	if (m_scene)
	{
		m_window.setView(m_window.getDefaultView());
		m_scene->render();
		m_window.display();
	}
}


Game::Game()
{
	loadSession();

	srand((uint32_t)time(nullptr));

	Scene::m_fadeBuffer.create(1920, 1080);
	Scene::p_window = &m_window;

#ifndef MNG_DIST
	//m_scene = std::make_unique<MainMenu>();
	m_scene = std::make_unique<Floor300>();
	//m_scene = std::make_unique<Classroom>();
#else
	m_scene = std::make_unique<MainMenu>();
#endif
}

Game::~Game()
{
	dumpSession();
}


void Game::run()
{
	while (m_window.isOpen())
	{
		update();
		render();
	}
}