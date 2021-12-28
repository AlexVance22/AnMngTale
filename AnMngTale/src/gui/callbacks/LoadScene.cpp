#include "PCH.h"
#include "LoadScene.h"

#include "core/Asserts.h"

#include "scenes/floors/Floor300.h"
#include "scenes/floors/Floor400.h"
#include "scenes/floors/Floor500.h"
#include "scenes/floors/Floor600.h"

#include "scenes/rooms/Classroom.h"

#include "scenes/outside/AulaYard.h"
#include "scenes/outside/Courtyard.h"
#include "scenes/outside/Basketball.h"


#define SET_SCENE(scene, spawn_x, spawn_y) Scene::s_spawnpoint = sf::Vector2f(spawn_x, spawn_y); menus->top().setNextScene<scene>()


void getSceneFromID(MenuStack* menus, uint32_t scene)
{
	switch (scene)
	{
	case 1:
		SET_SCENE(Courtyard, 250, 230);
		break;
	case 2:
		SET_SCENE(Floor300, 0, 0);
		break;
	case 3:
		SET_SCENE(Floor400, 0, 0);
		break;
	case 4:
		SET_SCENE(Floor500, 0, 0);
		break;
	case 5:
		SET_SCENE(Floor600, 0, 0);
		break;
	case 6:
		SET_SCENE(AulaYard, 0, 0);
		break;
	case 7:
		SET_SCENE(Basketball, 0, 0);
		break;
	case 8:
		SET_SCENE(Classroom, 0, 0);
		break;
	case 9:
		//SET_SCENE(Bathroom, 0, 0);
		break;
	default:
		MNG_ASSERT_MSG(false, "Invalid scene ID");
	}
}