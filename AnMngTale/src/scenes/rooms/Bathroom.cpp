#include "PCH.h"
#include "Bathroom.h"

#include "scenes/floors/Floor400.h"
#include "scenes/floors/Floor500.h"
#include "scenes/floors/Floor600.h"


void Bathroom::impl(const float deltaTime)
{

}


Bathroom::Bathroom() : Scene("bathroom")
{
	m_triggers["floors"].onCollide.bind(&Scene::loadScene<Floor400>, this);
}