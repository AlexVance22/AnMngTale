#include "PCH.h"
#include "Agenda.h"


static float normal(float val, float maxold)
{
	return val / maxold;
}

static float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}


std::unordered_map<std::string, Agenda::Quest> Agenda::s_quests = {
	{ { "Text1" }, { "Testing testing testing testing" } },
	{ { "Text2" }, { "Testing testing testing testing" } },
};


Agenda::Agenda()
{

}


void Agenda::postAssignment(const std::string& text)
{
	Alert a;
	a.sprite.setTexture(*p_texture);
	a.sprite.setPosition(-480.f, m_alerts.size() * 130.f + 40.f);

	a.title.setFont(*p_font);
	a.title.setPosition(a.sprite.getPosition() + sf::Vector2f(20, 5));
	a.title.setString("New assignment:");
	a.title.setCharacterSize(30);
	a.title.setFillColor(sf::Color(0, 0, 0));
	a.title.setStyle(sf::Text::Style::Bold);

	a.text.setFont(*p_font);
	a.text.setPosition(a.sprite.getPosition() + sf::Vector2f(20, 35));
	a.text.setString(text);
	a.text.setCharacterSize(25);
	a.text.setFillColor(sf::Color(0, 0, 0));

	m_alerts.push_back(a);
}

void Agenda::progAssignment()
{

}


void Agenda::update(const float deltaTime)
{
	if (!m_alerts.empty())
	{
		const float p1 = 0.7f, p2 = 4.f, p3 = 4.5f;

		for (auto& a : m_alerts)
		{
			if (a.timer < p1)
				a.sprite.setPosition(lerp(-480, 40, normal(a.timer, p1)), a.sprite.getPosition().y);
			else if (a.timer < p2)
				a.sprite.setPosition(40, a.sprite.getPosition().y);
			else if (a.timer < p3)
			{
				a.sprite.move(0, -1.f * deltaTime * 60.f);

				uint8_t alpha = (uint8_t)lerp(0, 255, normal(p3 - a.timer, p3 - p2));
				a.sprite.setColor(sf::Color(255, 255, 255, alpha));
				a.title.setFillColor(sf::Color(0, 0, 0, alpha));
				a.text.setFillColor(sf::Color(0, 0, 0, alpha));
			}

			a.title.setPosition(a.sprite.getPosition() + sf::Vector2f(20, 5));
			a.text.setPosition(a.sprite.getPosition() + sf::Vector2f(20, 40));
			a.timer += deltaTime;
		}

		if (m_alerts.front().timer >= p3)
			m_alerts.pop_front();
	}
}

void Agenda::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& a : m_alerts)
	{
		target.draw(a.sprite);
		target.draw(a.title);
		target.draw(a.text);
	}
}