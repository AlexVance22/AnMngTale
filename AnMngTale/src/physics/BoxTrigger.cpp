#include "PCH.h"
#include "BoxTrigger.h"


void BoxTrigger::triggerOnContains(sf::Vector2f point)
{
	if (contains(point))
		onCollide();
}

void BoxTrigger::triggerOnIntersects(sf::FloatRect rect)
{
	if (intersects(rect))
		onCollide();
}