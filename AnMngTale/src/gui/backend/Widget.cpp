#include "PCH.h"
#include "Widget.h"


namespace gui
{

void Widget::onMouseHover()
{

}

void Widget::onMouseExit()
{

}


void Widget::setLayout(sf::Vector2i position, sf::Vector2i size)
{
	m_background.setPosition((sf::Vector2f)position);

	m_boundingBox.left = position.x;
	m_boundingBox.top = position.y;
	m_boundingBox.width = size.x;
	m_boundingBox.height = size.y;
}


void Widget::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool Widget::isEnabled() const
{
	return m_enabled;
}


void Widget::setVisible(bool visible)
{
	m_visible = visible;
}

bool Widget::isVisible() const
{
	return m_visible;
}


void Widget::update(const float deltaTime)
{
	m_background.update(deltaTime);
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
}


}