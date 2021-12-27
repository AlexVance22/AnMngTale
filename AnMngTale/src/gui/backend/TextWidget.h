#pragma once

#include "Widget.h"


namespace gui
{

class TextWidget : public Widget
{
public:
	using Ptr = std::shared_ptr<TextWidget>;

protected:
	sf::Text m_text;

	void centerTextPosition();

	bool m_centered = true;

public:
	virtual void setLayout(sf::Vector2i position, sf::Vector2i size);

	void setCharacterSize(uint32_t size);
	void setTextColor(sf::Color color);
	void setFont(const sf::Font& font);
	void setString(const std::string& text);
	void setStyle(uint32_t style);

	void centerText();
	void setTextPosition(sf::Vector2f position);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}