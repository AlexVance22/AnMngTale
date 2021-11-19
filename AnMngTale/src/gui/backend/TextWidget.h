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

	void calculateTextPosition();

public:
	virtual void setLayout(sf::Vector2i position, sf::Vector2i size);
	virtual void setFont(const sf::Font& font, uint32_t size);
	void setString(const std::string& text);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}