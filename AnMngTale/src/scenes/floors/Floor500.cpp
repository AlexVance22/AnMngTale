#include "PCH.h"
#include "Floor500.h"

#include "Floor400.h"
#include "Floor600.h"
#include "scenes/rooms/Classroom.h"
#include "scenes/rooms/Bathroom.h"

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
		LOAD_SCENE("classroom", Classroom, 2, 0);

		LOAD_SCENE("bathroom1", Bathroom, 1, 0);
		LOAD_SCENE("bathroom2", Bathroom, 1, 1);

		LOAD_SCENE("f400top", Floor400, 370, -1460);
		LOAD_SCENE("f400bot", Floor400, 420, -500);

		LOAD_SCENE("f600top", Floor600, 370, -1460);
		LOAD_SCENE("f600bot", Floor600, 420, -500);
		break;
	}
}