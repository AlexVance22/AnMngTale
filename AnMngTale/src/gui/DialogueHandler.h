#pragma once

#include "backend/Callback.h"

#include "graphics/Sprite.h"


class DialogueHandler : public sf::Drawable
{
private:
	std::vector<std::deque<std::string>> m_alltext;
	std::deque<std::string>* p_strings = nullptr;

	Sprite m_background;
	sf::Text m_text;

	size_t m_charindex = 0;

	float m_chartime = 0.f;
	float m_totaltime = 0.f;

public:
	Callback onFinish;

public:
	DialogueHandler();

	bool isEndOfPage() const;
	bool isEndOfScript() const;
	bool isPlaying() const;

	void setPlaySpeed(float charPerSec);
	void begin(size_t snippet);
	void stop();

	void handleEvent(const sf::Event& event);
	void update(const float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	friend class Deserialiser;
};