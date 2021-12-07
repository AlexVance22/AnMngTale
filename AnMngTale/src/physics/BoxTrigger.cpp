#include "PCH.h"
#include "BoxTrigger.h"


void BoxTrigger::reset()
{
	m_used = false;
}


void BoxTrigger::triggerOnContains(sf::Vector2f point)
{
	if (contains(point))
	{
		if (!m_used)
		{
			m_used = true;
			onCollide();
		}
	}
	else if (autoreset)
		m_used = false;
}

void BoxTrigger::triggerOnIntersects(sf::FloatRect rect)
{
	if (intersects(rect))
	{
		if (!m_used)
		{
			m_used = true;
			onCollide();
		}
	}
	else if (autoreset)
		m_used = false;
}