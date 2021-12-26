#pragma once

#include "graphics/Sprite.h"


class Agenda : public sf::Drawable
{
private:
	struct Alert
	{
		Sprite sprite;
		sf::Text title;
		sf::Text text;
		float timer = 0.f;
	};

	std::deque<Alert> m_alerts;

	sf::Texture* p_texture = nullptr;
	sf::Font* p_font = nullptr;

	static std::unordered_map<std::string, std::string> s_quests;

public:
	Agenda();

	void postAssignment(const std::string& text);
	void progAssignment();

	void update(const float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	friend class Deserialiser;
};