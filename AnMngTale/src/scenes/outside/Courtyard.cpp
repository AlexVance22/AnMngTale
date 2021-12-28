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
		LOAD_SCENE("entrance", Floor300, 310, 190);
		break;
	case 1:
		LOAD_SCENE("entrance", Floor300, 310, 190);
		LOAD_SCENE("basketball", Basketball, 240, 135);
		LOAD_SCENE("aulayard", AulaYard, 240, 135);
		break;
	}
}