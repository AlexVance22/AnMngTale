#include "PCH.h"
#include "AulaYard.h"


void AulaYard::loadFlags()
{

}

void AulaYard::manageFlags()
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

void AulaYard::dumpFlags()
{

}


void AulaYard::impl(const float deltaTime)
{

}


AulaYard::AulaYard() : Scene("aulayard")
{

}