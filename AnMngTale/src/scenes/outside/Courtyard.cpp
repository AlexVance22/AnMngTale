#include "PCH.h"
#include "Courtyard.h"

#include "scenes/floors/Floor300.h"
#include "scenes/outside/Basketball.h"
#include "scenes/outside/AulaYard.h"

#include "gui/gui.h"


void Courtyard::impl(const float deltaTime)
{
	switch (m_state)
	{
	case 0:
		if (m_scripts[0].isEnd())
			m_progress = true;
		break;
	}
}


Courtyard::Courtyard() : Scene("courtyard")
{
	switch (m_state)
	{
	case 0:
		m_triggers["entrance"].onCollide.bind(&Scene::loadScene<Floor300>, this);
		break;
	case 1:
		m_triggers["entrance"].onCollide.bind(&Scene::loadScene<Floor300>, this);
		m_triggers["basketball"].onCollide.bind(&Scene::loadScene<Basketball>, this);
		m_triggers["aulayard"].onCollide.bind(&Scene::loadScene<AulaYard>, this);
		break;
	}
}