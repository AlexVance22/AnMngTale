#include "PCH.h"
#include "Slider.h"

#include "io/JsonUtils.h"


namespace gui
{

bool Slider::slide(int x)
{
	const float cache = m_value;

	const int local = x - m_boundingBox.left;
	const int clamped = std::clamp(local, 0, (int)m_width - 1);
	m_value = (float)clamped / (float)m_width;

	if (m_target)
		*m_target = m_value * m_scale;

	m_background.setPosition((float)(m_boundingBox.left + clamped), (float)m_boundingBox.top);
	shape.setPosition((float)(m_boundingBox.left + clamped), (float)m_boundingBox.top);

	return cache != m_value;
}


void Slider::load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts)
{
	setLayout(JsonToVec2<int>(data["position"]), JsonToVec2<int>(data["size"]));
}

void Slider::load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts, const rapidjson::Value& preset)
{
	setLayout(JsonToVec2<int>(data["position"]), JsonToVec2<int>(preset["size"]));
}


void Slider::setRawValue(float val)
{
	const float clamped = std::clamp(val, 0.f, 1.f);
	const int local = (int)(clamped * (float)m_width);
	m_value = clamped;

	if (m_target)
		*m_target = m_value * m_scale;

	m_background.setPosition((float)(m_boundingBox.left + local), (float)m_boundingBox.top);
	shape.setPosition((float)(m_boundingBox.left + local), (float)m_boundingBox.top);
}

float Slider::getRawValue() const
{
	return m_value;
}


void Slider::setScaleTarget(float& target, float scale)
{
	m_target = &target;
	m_scale = scale;

	m_value = target / scale;

	m_background.setPosition((float)(m_boundingBox.left + m_value * m_width), (float)m_boundingBox.top);
	shape.setPosition((float)(m_boundingBox.left + m_value * m_width), (float)m_boundingBox.top);
}

void Slider::setLayout(sf::Vector2i position, sf::Vector2i size)
{
	m_background.setPosition((sf::Vector2f)position);
	m_background.move(size.x * m_value, 0);

	shape.setPosition((sf::Vector2f)position);
	shape.setSize(sf::Vector2f(size.x * 0.06f, (float)size.y));
	shape.move(size.x * m_value, 0);

	m_line[0].position = (sf::Vector2f)position + sf::Vector2f(0.f, size.y * 0.5f);
	m_line[1].position = (sf::Vector2f)position + sf::Vector2f((float)size.x, size.y * 0.5f);

	m_boundingBox = sf::IntRect(position, size);

	m_width = m_boundingBox.width - (int)(m_boundingBox.width * 0.06f) + 1;
}


void Slider::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		if (m_boundingBox.contains(event.mouseButton.x, event.mouseButton.y))
		{
			m_held = true;
			if (slide(event.mouseButton.x))
				onValueChange();
		}
		break;
	case sf::Event::MouseButtonReleased:
		m_held = false;
		break;
	case sf::Event::MouseMoved:
		if (m_held)
		{
			if (slide(event.mouseMove.x))
				onValueChange();
		}
		break;
	}
}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_line, 2, sf::Lines);
	target.draw(m_background);
	target.draw(shape);
}

}