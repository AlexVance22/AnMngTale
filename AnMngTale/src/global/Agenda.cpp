#include "PCH.h"
#include "Agenda.h"


Agenda::Agenda()
{

}


void Agenda::postAssignment()
{

}

void Agenda::progAssignment()
{

}


void Agenda::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& a : m_assignments)
		target.draw(a.sprite);
}