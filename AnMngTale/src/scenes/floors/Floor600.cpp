#include "PCH.h"
#include "Floor600.h"

#include "entities/Player.h"


void Floor600::loadFlags()
{

}

void Floor600::dumpFlags()
{

}


void Floor600::impl(const float deltaTime)
{
	switch (m_state)
	{
	case 0:
		for (const auto& script : m_scripts)
		{

		}
		break;
	case 1:
		break;
	}

	sf::Vector2f playerRelTex = (m_player->getPosition() + m_player->getSize() * 0.5f) - m_topLayerPos;
	sf::Vector2f normalised(playerRelTex.x / m_topLayerSize.x, playerRelTex.y / m_topLayerSize.y);
	m_shaders["xray"].setUniform("player_position", normalised);
}


Floor600::Floor600() : Scene("floor600")
{

}