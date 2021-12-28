#include "PCH.h"
#include "Bathroom.h"

#include "scenes/floors/Floor300.h"
#include "scenes/floors/Floor400.h"
#include "scenes/floors/Floor500.h"
#include "scenes/floors/Floor600.h"


void Bathroom::impl(const float deltaTime)
{

}


Bathroom::Bathroom() : Scene("bathroom")
{
	switch ((int)s_spawnpoint.x)
	{
	case 0:
		LOAD_SCENE("floors", Floor300, 420, -940 - s_spawnpoint.y * 121);
		break;
	case 1:
		LOAD_SCENE("floors", Floor400, 420, -940 - s_spawnpoint.y * 121);
		break;
	case 2:
		LOAD_SCENE("floors", Floor500, 420, -940 - s_spawnpoint.y * 121);
		break;
	case 3:
		LOAD_SCENE("floors", Floor600, 420, -940 - s_spawnpoint.y * 121);
		break;
	}
}