#include "PCH.h"
#include "TextWidget.h"


namespace gui
{

void TextWidget::calculateTextPosition()
{
	m_text.setPosition(m_boundingBox.left + m_boundingBox.width / 2 - m_text.getLocalBounds().width / 2,
					   m_boundingBox.top + m_boundingBox.height / 2 - m_text.getLocalBounds().height / 2 - 5);
}


void TextWidget::setLayout(sf::Vector2i position, sf::Vector2i size)
{
	m_background.setPosition((sf::Vector2f)position);

	m_boundingBox.left = position.x;
	m_boundingBox.top = position.y;
	m_boundingBox.width = size.x;
	m_boundingBox.height = size.y;

	calculateTextPosition();
}

void TextWidget::setFont(const sf::Font& font, uint32_t size)
{
	m_text.setFont(font);
	m_text.setCharacterSize(size);
	calculateTextPosition();
}

void TextWidget::setString(const std::string& text)
{
	m_text.setString(text);
	calculateTextPosition();
}


void TextWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_text);
}

}