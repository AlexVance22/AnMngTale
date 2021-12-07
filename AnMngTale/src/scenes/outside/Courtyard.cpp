#include "PCH.h"
#include "Courtyard.h"

#include "scenes/floors/Floor300.h"
#include "scenes/outside/Basketball.h"
#include "scenes/outside/AulaYard.h"

#include "gui/gui.h"
#include "io/JsonUtils.h"


void Courtyard::loadFlags()
{

}

void Courtyard::dumpFlags()
{
	rapidjson::Document doc;
	loadjson(doc, "config/saves/state0.json");

	switch (m_state)
	{
	case 0:
		doc["courtyard"].SetUint(1);
		break;
	}

	dumpjson(doc, "config/saves/state0.json");
}


void Courtyard::impl(const float deltaTime)
{

}


Courtyard::Courtyard() : Scene("courtyard")
{
	m_triggers["entrance"].onCollide.bind(&Scene::loadScene<Floor300>, this);
	m_triggers["basketball"].onCollide.bind(&Scene::loadScene<Basketball>, this);
	m_triggers["aulayard"].onCollide.bind(&Scene::loadScene<AulaYard>, this);
}

Courtyard::~Courtyard()
{
	dumpFlags();
}