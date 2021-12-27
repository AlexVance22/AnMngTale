#include "PCH.h"
#include "TextWidget.h"


namespace gui
{

void TextWidget::centerTextPosition()
{
	m_text.setPosition(floor(m_boundingBox.left + m_boundingBox.width / 2 - m_text.getLocalBounds().width / 2),
					   floor(m_boundingBox.top + m_boundingBox.height / 2 - m_text.getLocalBounds().height / 2 - m_boundingBox.height * 0.1f));
}


void TextWidget::setLayout(sf::Vector2i position, sf::Vector2i size)
{
	m_background.setPosition((sf::Vector2f)position);

	m_boundingBox.left = position.x;
	m_boundingBox.top = position.y;
	m_boundingBox.width = size.x;
	m_boundingBox.height = size.y;

	if (m_centered)
		centerTextPosition();
}


void TextWidget::setCharacterSize(uint32_t size)
{
	m_text.setCharacterSize(size);

	if (m_centered)
		centerTextPosition();
}

void TextWidget::setTextColor(sf::Color color)
{
	m_text.setFillColor(color);
}

void TextWidget::setFont(const sf::Font& font)
{
	m_text.setFont(font);

	if (m_centered)
		centerTextPosition();
}

void TextWidget::setString(const std::string& text)
{
	m_text.setString(text);

	if (m_centered)
		centerTextPosition();
}

void TextWidget::setStyle(uint32_t style)
{
	m_text.setStyle(style);
}


void TextWidget::centerText()
{
	m_centered = true;

	centerTextPosition();
}

void TextWidget::setTextPosition(sf::Vector2f position)
{
	m_centered = false;

	m_text.setPosition(m_boundingBox.left + position.x, m_boundingBox.top + position.y);
}


void TextWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_text);
}

}