#pragma once

#include "gui/backend/Callback.h"


struct BoxTrigger : public sf::FloatRect
{
	using sf::FloatRect::Rect;

	Callback onCollide;

	void triggerOnContains(sf::Vector2f point);
	void triggerOnIntersects(sf::FloatRect rect);
};