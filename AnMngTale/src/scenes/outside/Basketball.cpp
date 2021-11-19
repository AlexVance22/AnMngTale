#include "PCH.h"
#include "Basketball.h"


void Basketball::loadFlags()
{

}

void Basketball::manageFlags()
{
	switch (m_state)
	{
	case AreaState::INTRO:
		//if (seq->getSignal() == 0)
			//menus.push(new NameSelect(&menus));
		break;
	case AreaState::DEFAULT:
		break;
	}
}

void Basketball::dumpFlags()
{

}


void Basketball::handleEvents()
{
	sf::Event event;
	while (p_window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Space: case sf::Keyboard::Return:
				//seq->skipDia();
				break;
			case sf::Keyboard::Escape:
				m_overlays.emplace(p_window, &m_overlays, "res/menus/quit.json");
				break;
			case sf::Keyboard::E:
				m_overlays.emplace(p_window, &m_overlays, "res/menus/agenda.json");
			}
			break;
		}
	}
}

void Basketball::impl(const float deltaTime)
{

}


Basketball::Basketball() : Scene("basketball")
{

}