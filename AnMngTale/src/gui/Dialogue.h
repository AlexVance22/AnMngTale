#pragma once

#include "backend/Callback.h"

#include "graphics/Sprite.h"


class Dialogue : public sf::Drawable
{
private:
	std::vector<std::deque<std::string>> m_alltext;
	std::deque<std::string> m_strings;

	Sprite m_background;
	sf::Text m_text;

	int m_charindex = -1;
	bool m_playing = false;

	float m_chartime = 0.f;
	float m_totaltime = 0.f;

public:
	Callback onFinish;

public:
	Dialogue();

	bool isEndOfPage() const;
	bool isEndOfScript() const;
	bool isPlaying() const;

	void setPlaySpeed(float charPerSec);
	void begin(size_t snippet);
	void clear();

	void handleEvent(const sf::Event& event);
	void update(const float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	friend class Deserialiser;
};