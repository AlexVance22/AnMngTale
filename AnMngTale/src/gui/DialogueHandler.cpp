#include "PCH.h"
#include "DialogueHandler.h"


DialogueHandler::DialogueHandler()
{
	m_background.setPosition(0, 830);

	m_text.setPosition(30, 850);
	m_text.setCharacterSize(30);
	m_text.setFillColor(sf::Color(0, 0, 0, 255));
}


bool DialogueHandler::isEndOfPage() const
{
	return m_charindex == p_strings->front().size() - 1;
}

bool DialogueHandler::isEndOfScript() const
{
	return p_strings->empty();
}

bool DialogueHandler::isPlaying() const
{
	return (bool)p_strings;
}


void DialogueHandler::setPlaySpeed(float charPerSec)
{
	m_chartime = 1.f / charPerSec;
	m_totaltime = m_chartime;
}

void DialogueHandler::begin(size_t snippet)
{
	stop();

	p_strings = &m_alltext[snippet];

	m_text.setString(p_strings->front()[0]);
}

void DialogueHandler::stop()
{
	p_strings = nullptr;

	m_text.setString("");

	m_charindex = 0;
	m_totaltime = 0.f;
}


void DialogueHandler::handleEvent(const sf::Event& event)
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
						m_charindex = p_strings->front().size() - 1;
						m_text.setString(p_strings->front());
					}
					else
					{
						m_charindex = 0;
						p_strings->pop_front();

						if (!isEndOfScript())
							m_text.setString(p_strings->front()[0]);
						else
						{
							onFinish();
							p_strings = nullptr;
						}
					}
				}
			}
			break;
		}
	}
}

void DialogueHandler::update(const float deltaTime)
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
					m_text.setString(m_text.getString() + p_strings->front()[m_charindex]);
				}
			}
		}
	}
}

void DialogueHandler::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isPlaying())
	{
		target.draw(m_background);
		target.draw(m_text);
	}
}