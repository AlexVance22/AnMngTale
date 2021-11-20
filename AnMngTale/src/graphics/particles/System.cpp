#include "PCH.h"
#include "System.h"


namespace particles
{

	void System::p_setPosition(size_t idx, sf::Vector2f position)
	{
		m_va[idx * 4 + 0].position += position;
		m_va[idx * 4 + 1].position += position;
		m_va[idx * 4 + 2].position += position;
		m_va[idx * 4 + 3].position += position;
	}

	sf::Vector2f System::p_getPosition(size_t idx) const
	{
		return m_va[idx * 4].position;
	}


	void System::p_setSize(size_t idx, sf::Vector2f size)
	{
		const sf::Vector2f pos = m_va[idx * 4].position;

		m_va[idx * 4 + 1].position.x = pos.x + size.x;
		m_va[idx * 4 + 2].position = pos + size;
		m_va[idx * 4 + 3].position.y = pos.y + size.y;
	}

	sf::Vector2f System::p_getSize(size_t idx) const
	{
		return m_va[idx * 4 + 2].position - m_va[idx * 4 + 0].position;
	}


	void System::p_move(size_t idx, sf::Vector2f movement)
	{
		p_setPosition(idx, p_getPosition(idx) + movement);
	}

	sf::FloatRect System::p_getBounds(size_t idx) const
	{
		return sf::FloatRect(p_getPosition(idx), p_getSize(idx));
	}


	void System::p_erase(size_t idx)
	{
		m_particles.erase(m_particles.begin() + idx);

		m_va.erase(m_va.begin() + (idx * 4) + 0);
		m_va.erase(m_va.begin() + (idx * 4) + 1);
		m_va.erase(m_va.begin() + (idx * 4) + 2);
		m_va.erase(m_va.begin() + (idx * 4) + 3);
	}


	System::System(size_t size)
	{
		m_va.resize(size * 4);
	}


	void System::setBoundary(sf::FloatRect boundary)
	{
		m_bounds = boundary;
		for (size_t i = m_particles.size() + 1; i >= 0; i--)
		{
			if (p_getBounds(i).intersects(m_bounds))
				p_erase(i);
		}
	}


	void System::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_va.data(), m_va.size(), sf::Quads);
	}
}