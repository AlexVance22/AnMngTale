#include "PCH.h"
#include "Dialogue.h"


namespace gui
{

bool Dialogue::isPlaying()
{
	return !(m_charindex == m_alltext[m_pageindex].size());
}

bool Dialogue::isTerminated()
{
	return m_pageindex == m_alltext.size();
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
				m_charindex = m_alltext[m_pageindex].size();
			else
			{
				m_charindex = 0;
				m_pageindex++;
			}
			break;
		}
	}
}

void Dialogue::update(const float deltaTime)
{
	if (isPlaying())
	{
		if (m_totaltime >= m_chartime)
		{
			m_charindex++;
			m_totaltime -= m_chartime;
			m_text.setString(m_text.getString() + m_alltext[m_pageindex][m_charindex]);
		}
		m_totaltime += deltaTime;
	}
}

}