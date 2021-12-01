#include "PCH.h"
#include "Dialogue.h"

#include "core/Asserts.h"
#include "core/JsonCasts.h"


void loadjson(rapidjson::Document& doc, const std::string& filepath);


namespace gui
{

bool Dialogue::isEndOfPage()
{
	return m_charindex == (*m_alltext)[m_pageindex].size() - 1;
}

bool Dialogue::isEndOfScript()
{
	return m_pageindex == m_alltext->size();
}


void Dialogue::setPlaySpeed(float charPerSec)
{
	m_chartime = 1.f / charPerSec;
	m_totaltime = m_chartime;
}

void Dialogue::setText(const std::vector<std::string>& text)
{
	m_alltext = &text;

	m_text.setString((*m_alltext)[0][0]);
}


void Dialogue::load(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
	const std::unordered_map<std::string, sf::Font>& fonts)
{
	setLayout(JsonToVec2<int>(data["position"]), JsonToVec2<int>(data["size"]));

	MNG_ASSERT_SLIM(fonts.find(data["font"].GetString()) != fonts.end());
	setFont(fonts.at(data["font"].GetString()), data["fontsize"].GetUint());

	m_text.setFillColor(JsonToColor(data["fontcolor"]));

	setTextCentered(false);
	setTextPosition(m_background.getPosition() + sf::Vector2f(30, 20));

	MNG_ASSERT_SLIM(textures.find(data["texture"].GetString()) != textures.end());
	m_background.setTexture(textures.at(data["texture"].GetString()));
}

void Dialogue::loadPreset(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
	const std::unordered_map<std::string, sf::Font>& fonts, const rapidjson::Value& preset)
{

}


void Dialogue::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Return: case sf::Keyboard::Space:
			if (!isEndOfScript())
			{
				if (!isEndOfPage())
				{
					m_charindex = (*m_alltext)[m_pageindex].size() - 1;
					m_text.setString((*m_alltext)[m_pageindex]);
				}
				else
				{
					m_charindex = 0;
					m_pageindex++;

					if (!isEndOfScript())
						m_text.setString((*m_alltext)[m_pageindex][0]);
					else
						onFinish();
				}
			}
			break;
		}
	}
}

void Dialogue::update(const float deltaTime)
{
	if (!isEndOfScript())
	{
		m_totaltime -= deltaTime;

		if (m_totaltime < 0)
		{
			m_totaltime = m_chartime;

			if (!isEndOfPage())
			{
				m_charindex++;
				m_text.setString(m_text.getString() + (*m_alltext)[m_pageindex][m_charindex]);
			}
		}
	}
}

}