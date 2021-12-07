#include "PCH.h"
#include "Floor300.h"

#include "entities/Player.h"


void Floor300::loadFlags()
{

}

void Floor300::dumpFlags()
{

}


void Floor300::impl(const float deltaTime)
{
	switch (m_state)
	{
	case 0:
		for (const auto& script : m_scripts)
		{
			if (script.output() == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::E))
				m_triggers["interact1"].reset();
			if (script.output() == 1)
				m_triggers["interact2"].reset();
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

}