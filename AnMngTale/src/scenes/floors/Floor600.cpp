#include "PCH.h"
#include "Floor600.h"

#include "Floor500.h"

#include "entities/Player.h"


void Floor600::impl(const float deltaTime)
{
	switch (m_state)
	{
	case 0:
		break;
	}

	sf::Vector2f playerRelTex = (m_player->getPosition() + m_player->getSize() * 0.5f) - m_topLayerPos;
	sf::Vector2f normalised(playerRelTex.x / m_topLayerSize.x, playerRelTex.y / m_topLayerSize.y);
	m_shaders["xray"].setUniform("player_position", normalised);
}


Floor600::Floor600() : Scene("floor600")
{
	switch (m_state)
	{
	case 0:
		//m_triggers["classroom"].onCollide.bind(&Scene::loadScene<Classroom>, this);

		m_triggers["f500top"].onCollide.bind(&Scene::loadScene<Floor500>, this);
		m_triggers["f500bot"].onCollide.bind(&Scene::loadScene<Floor500>, this);
		break;
	}
}