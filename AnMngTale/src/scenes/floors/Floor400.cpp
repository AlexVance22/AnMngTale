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
		if (m_scripts[0].output() == 1)
			m_triggers["interact2"].reset();
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
		LOAD_SCENE("classroom", Classroom, 1, 0);
		break;
	case 1:
		LOAD_SCENE("classroom", Classroom, 1, 0);

		LOAD_SCENE("bathroom1", Bathroom, 1, 0);
		LOAD_SCENE("bathroom2", Bathroom, 1, 1);

		LOAD_SCENE("f300top", Floor300, 370, -1460);
		LOAD_SCENE("f300bot", Floor300, 420, -500);

		LOAD_SCENE("f500top", Floor500, 370, -1460);
		LOAD_SCENE("f500bot", Floor500, 420, -500);
		break;
	}
}