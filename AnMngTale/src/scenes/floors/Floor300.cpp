#include "PCH.h"
#include "Floor300.h"

#include "entities/Player.h"


void Floor300::loadFlags()
{

}

void Floor300::manageFlags()
{
	switch (m_state)
	{
	case AreaState::INTRO:
		//if (seq->getSignal() == 0)
			//menus.push(new NameSelect(&menus));
		break;
	case AreaState::DEFAULT:
		break;
	}
}

void Floor300::dumpFlags()
{

}


void Floor300::impl(const float deltaTime)
{
	sf::Vector2f playerRelTex = (m_player->getPosition() + m_player->getSize() * 0.5f) - m_topLayerPos;
	sf::Vector2f normalised(playerRelTex.x / m_topLayerSize.x, playerRelTex.y / m_topLayerSize.y);
	m_shaders["xray"].setUniform("player_position", normalised);
}


Floor300::Floor300() : Scene("floor300")
{

}