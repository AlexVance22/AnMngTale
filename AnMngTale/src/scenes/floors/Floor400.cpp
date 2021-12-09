#include "PCH.h"
#include "Floor400.h"

#include "entities/Player.h"


void Floor400::impl(const float deltaTime)
{
	switch (m_state)
	{
	case 0:
		if (m_scripts[0].output() == 0)
			m_progress = true;
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