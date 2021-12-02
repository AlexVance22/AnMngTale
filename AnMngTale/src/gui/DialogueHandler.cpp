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
	return m_charindex == (*m_alltext)[m_pageindex].size() - 1;
}

bool DialogueHandler::isEndOfScript() const
{
	return m_pageindex == m_alltext->size();
}

bool DialogueHandler::isPlaying() const
{
	return (bool)m_alltext;
}


void DialogueHandler::setPlaySpeed(float charPerSec)
{
	m_chartime = 1.f / charPerSec;
	m_totaltime = m_chartime;
}

void DialogueHandler::begin(const std::vector<std::string>& text)
{
	m_alltext = &text;

	m_text.setString((*m_alltext)[0][0]);
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
						{
							onFinish();
							m_alltext = nullptr;
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
					m_text.setString(m_text.getString() + (*m_alltext)[m_pageindex][m_charindex]);
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