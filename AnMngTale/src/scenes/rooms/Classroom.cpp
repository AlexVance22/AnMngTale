#include "PCH.h"
#include "Classroom.h"

#include "scenes/floors/Floor400.h"
#include "scenes/floors/Floor500.h"
#include "scenes/floors/Floor600.h"


void Classroom::impl(const float deltaTime)
{

}


Classroom::Classroom() : Scene("classroom")
{
	switch ((int)s_spawnpoint.x)
	{
	case 1:
		LOAD_SCENE("floors", Floor400, 420, -1249);
		break;
	case 2:
		LOAD_SCENE("floors", Floor500, 420, -1249);
		break;
	case 3:
		LOAD_SCENE("floors", Floor600, 420, -1249);
		break;
	}
}