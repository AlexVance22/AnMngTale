#include "PCH.h"
#include "Classroom.h"

#include "scenes/floors/Floor400.h"


void Classroom::impl(const float deltaTime)
{

}


Classroom::Classroom() : Scene("classroom")
{
	m_triggers["floors"].onCollide.bind(&Scene::loadScene<Floor400>, this);
}