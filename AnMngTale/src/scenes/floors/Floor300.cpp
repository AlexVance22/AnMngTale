#include "PCH.h"
#include "Floor300.h"

#include "entities/Player.h"

#include "scenes/floors/Floor400.h"
#include "scenes/outside/Basketball.h"
#include "scenes/outside/Courtyard.h"
#include "scenes/rooms/Library.h"

#include "global/Agenda.h"


void Floor300::handleEventSpecial(const sf::Event& event)
{
	switch (m_state)
	{
	case 0:
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::E:
				if (m_scripts[0].output() == 0)
					m_triggers["interact1"].reset();
				[[fallthrough]];
			case sf::Keyboard::Escape:
				if (m_scripts[0].output() == 1)
					m_triggers["interact1"].reset();
			}
			break;
		}
		break;
	case 1:
		break;
	}
}

void Floor300::impl(const float deltaTime)
{
	switch (m_state)
	{
	case 0:
		switch (m_scripts[0].output())
		{
		case 3:
			m_agenda.postAssignment("Open your Agenda by pressing 'E'");
			break;
		case 4:
			m_agenda.postAssignment("Find the classroom");
			break;
		case 2:
			m_triggers["interact2"].reset();
			m_progress = true;
			break;
		}
		break;
	case 1:
		break;
	}

	sf::Vector2f playerRelTex = (m_player->getPosition() + m_player->getSize() * 0.5f) - m_topLayerPos;
	sf::Vector2f normalised(playerRelTex.x / m_topLayerSize.x, playerRelTex.y / m_topLayerSize.y);
	m_shaders["xray"].setUniform("player_position", normalised);
}


Floor300::Floor300() : Scene("floor300")
{
	switch (m_state)
	{
	case 0:
		LOAD_SCENE("f400top", Floor400, 370, -1460);
		LOAD_SCENE("f400bot", Floor400, 420, -500);

		LOAD_SCENE("library", Library, 0, 0);
		break;
	case 1:
		LOAD_SCENE("f400top", Floor400, 370, -1460);
		LOAD_SCENE("f400bot", Floor400, 420, -500);

		LOAD_SCENE("courtyard", Courtyard, 321, 84);
		LOAD_SCENE("basketball", Basketball, 345, 206);

		LOAD_SCENE("library", Library, 0, 0);
		break;
	}
}