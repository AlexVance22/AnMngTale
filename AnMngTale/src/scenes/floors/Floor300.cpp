#include "PCH.h"
#include "Floor300.h"

#include "entities/Player.h"

#include "scenes/floors/Floor400.h"
#include "scenes/outside/Basketball.h"
#include "scenes/outside/Courtyard.h"

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
			m_agenda.postAssignment("Press 'E'");
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
		m_triggers["f400top"].onCollide.bind(&Scene::loadScene<Floor400>, this);
		m_triggers["f400bot"].onCollide.bind(&Scene::loadScene<Floor400>, this);
		break;
	case 1:
		m_triggers["f400top"].onCollide.bind(&Scene::loadScene<Floor400>, this);
		m_triggers["f400bot"].onCollide.bind(&Scene::loadScene<Floor400>, this);
		m_triggers["courtyard"].onCollide.bind(&Scene::loadScene<Courtyard>, this);
		m_triggers["basketball"].onCollide.bind(&Scene::loadScene<Basketball>, this);
		break;
	}
}