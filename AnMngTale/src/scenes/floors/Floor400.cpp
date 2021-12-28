#include "PCH.h"
#include "Floor400.h"

#include "Floor300.h"
#include "Floor500.h"
#include "scenes/rooms/Classroom.h"
#include "scenes/rooms/Bathroom.h"

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
	switch (m_state)
	{
	case 0:
		m_triggers["classroom"].onCollide.bind(&Scene::loadScene<Classroom>, this);
		break;
	case 1:
		m_triggers["classroom"].onCollide.bind(&Scene::loadScene<Classroom>, this);

		m_triggers["bathroom1"].onCollide.bind(&Scene::loadScene<Bathroom>, this);
		m_triggers["bathroom2"].onCollide.bind(&Scene::loadScene<Bathroom>, this);

		m_triggers["f300top"].onCollide.bind(&Scene::loadScene<Floor300>, this);
		m_triggers["f300bot"].onCollide.bind(&Scene::loadScene<Floor300>, this);

		m_triggers["f500top"].onCollide.bind(&Scene::loadScene<Floor500>, this);
		m_triggers["f500bot"].onCollide.bind(&Scene::loadScene<Floor500>, this);
		break;
	}
}