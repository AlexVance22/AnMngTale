#include "PCH.h"
#include "Floor400.h"

#include "entities/Player.h"


void Floor400::loadFlags()
{

}

void Floor400::dumpFlags()
{

}


void Floor400::impl(const float deltaTime)
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


Floor400::Floor400() : Scene("floor400")
{

}