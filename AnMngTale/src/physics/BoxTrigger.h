#pragma once

#include "gui/backend/Callback.h"


struct BoxTrigger : public sf::FloatRect
{
private:
	bool m_used = false;

public:
	using sf::FloatRect::Rect;

	bool autoreset = false;
	Callback onCollide;

	void reset();

	void triggerOnContains(sf::Vector2f point);
	void triggerOnIntersects(sf::FloatRect rect);
};