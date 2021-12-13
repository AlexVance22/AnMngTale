#include "PCH.h"
#include "Floor500.h"

#include "Floor400.h"
#include "Floor600.h"

#include "entities/Player.h"


void Floor500::impl(const float deltaTime)
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


Floor500::Floor500() : Scene("floor500")
{
	switch (m_state)
	{
	case 0:
		//m_triggers["classroom"].onCollide.bind(&Scene::loadScene<Classroom>, this);

		m_triggers["f400top"].onCollide.bind(&Scene::loadScene<Floor400>, this);
		m_triggers["f400bot"].onCollide.bind(&Scene::loadScene<Floor400>, this);

		m_triggers["f600top"].onCollide.bind(&Scene::loadScene<Floor600>, this);
		m_triggers["f600bot"].onCollide.bind(&Scene::loadScene<Floor600>, this);
		break;
	}
}