#include "PCH.h"
#include "Dialogue.h"


Dialogue::Dialogue()
{
	m_background.setPosition(0, 830);

	m_text.setPosition(30, 850);
	m_text.setCharacterSize(35);
	m_text.setFillColor(sf::Color(0, 0, 0, 255));
}


bool Dialogue::isEndOfPage() const
{
	return m_charindex == m_strings.front().size() - 1;
}

bool Dialogue::isEndOfScript() const
{
	return m_strings.empty();
}

bool Dialogue::isPlaying() const
{
	return m_playing;
}


void Dialogue::setPlaySpeed(float charPerSec)
{
	m_chartime = 1.f / charPerSec;
	m_totaltime = m_chartime;
}

void Dialogue::begin(size_t snippet)
{
	m_charindex = -1;
	m_totaltime = 0.f;

	m_strings = std::move(m_alltext[snippet]);

	m_playing = true;
}

void Dialogue::clear()
{
	m_playing = false;

	m_text.setString("");

	m_charindex = -1;
	m_totaltime = 0.f;

	m_alltext.clear();
	m_strings.clear();
}


void Dialogue::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Return: case sf::Keyboard::Space:
			if (isPlaying())
			{
				if (!isEndOfScript())
				{
					if (!isEndOfPage())
					{
						std::string res = m_strings.front();
						res.erase(std::remove(res.begin(), res.end(), '_'), res.end());
						m_charindex = (int)m_strings.front().size() - 1;
						m_text.setString(res);
					}
					else
					{
						m_charindex = -1;
						m_strings.pop_front();
						m_text.setString("");

						if (isEndOfScript())
						{
							onFinish();
							m_playing = false;
						}
					}
				}
			}
			break;
		}
	}
}

void Dialogue::update(const float deltaTime)
{
	if (isPlaying())
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

					if (m_strings.front()[m_charindex] == '_')
						m_totaltime += 0.1f;
					else
						m_text.setString(m_text.getString() + m_strings.front()[m_charindex]);
				}
			}
		}
	}
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isPlaying())
	{
		target.draw(m_background);
		target.draw(m_text);
	}
}