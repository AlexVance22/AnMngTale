#pragma once

#include "graphics/Sprite.h"


class Agenda : public sf::Drawable
{
private:
	struct Task
	{
		Sprite sprite;
	};

	std::deque<Task> m_assignments;

	sf::Texture m_texture;

public:
	Agenda();

	void postAssignment();
	void progAssignment();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	friend class Deserialiser;
};