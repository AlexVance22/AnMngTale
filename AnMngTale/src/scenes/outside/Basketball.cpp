#include "PCH.h"
#include "Basketball.h"


void Basketball::loadFlags()
{

}

void Basketball::manageFlags()
{
	switch (m_state)
	{
	case AreaState::INTRO:
		//if (seq->getSignal() == 0)
			//menus.push(new NameSelect(&menus));
		break;
	case AreaState::DEFAULT:
		break;
	}
}

void Basketball::dumpFlags()
{

}


void Basketball::impl(const float deltaTime)
{

}


Basketball::Basketball() : Scene("basketball")
{

}