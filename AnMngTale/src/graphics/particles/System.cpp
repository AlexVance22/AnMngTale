#include "PCH.h"
#include "System.h"


namespace particles
{

	void System::p_setPosition(size_t idx, sf::Vector2f position)
	{
		const sf::Vector2f size = p_getSize(idx);

		m_va[idx * 4].position = position;

		p_setSize(idx, size);
	}

	sf::Vector2f System::p_getPosition(size_t idx) const
	{
		return m_va[idx * 4].position;
	}


	void System::p_setSize(size_t idx, sf::Vector2f size)
	{
		const sf::Vector2f pos = m_va[idx * 4].position;

		m_va[idx * 4 + 1].position = sf::Vector2f(pos.x + size.x, pos.y);
		m_va[idx * 4 + 2].position = pos + size;
		m_va[idx * 4 + 3].position = sf::Vector2f(pos.x, pos.y + size.y);
	}

	sf::Vector2f System::p_getSize(size_t idx) const
	{
		return m_va[idx * 4 + 2].position - m_va[idx * 4 + 0].position;
	}


	void System::p_setColor(size_t idx, sf::Color color)
	{
		m_va[idx * 4 + 0].color = color;
		m_va[idx * 4 + 1].color = color;
		m_va[idx * 4 + 2].color = color;
		m_va[idx * 4 + 3].color = color;
	}

	void System::p_setTextureSize(size_t idx, sf::Vector2f size)
	{
		m_va[idx * 4 + 0].texCoords = sf::Vector2f(0, 0);
		m_va[idx * 4 + 1].texCoords = sf::Vector2f(size.x, 0);
		m_va[idx * 4 + 2].texCoords = size;
		m_va[idx * 4 + 3].texCoords = sf::Vector2f(0, size.y);
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
		m_va.erase(m_va.begin() + (idx * 4) + 3);
		m_va.erase(m_va.begin() + (idx * 4) + 2);
		m_va.erase(m_va.begin() + (idx * 4) + 1);
		m_va.erase(m_va.begin() + (idx * 4) + 0);
	}


	void System::cull()
	{
		for (int64_t i = ((int64_t)m_va.size() / 4) - 1; i >= 0; i--)
		{
			if (!p_getBounds(i).intersects(m_bounds))
				p_erase(i);
		}
	}


	size_t System::emplace(int idx)
	{
		if (idx == -1)
		{
			idx = m_va.size() / 4;

			for (size_t i = 0; i < 4; i++)
				m_va.emplace_back();
		}

		sf::Vector2f size = m_size;
		if (m_randSize)
		{
			int num = rand();
			size.x += num % (int)(m_dsize.x + 1) * (2 * (m_dsize.x > 0) - 1);
			size.y += num % (int)(m_dsize.y + 1) * (2 * (m_dsize.y > 0) - 1);
		}

		p_setSize(idx, size);
		p_setColor(idx, m_color);
		p_setTextureSize(idx, m_texsize);

		return idx;
	}


	System::System(sf::FloatRect rect) : m_bounds(rect), m_color(255, 255, 255)
	{

	}


	void System::setColor(sf::Color color)
	{
		m_color = color;

		for (size_t i = 0; i < (m_va.size() / 4); i++)
			p_setColor(i, m_color);
	}

	void System::setTexture(const sf::Texture& tex)
	{
		m_tex = &tex;
		m_texsize = (sf::Vector2f)tex.getSize();

		for (size_t i = 0; i < (m_va.size() / 4); i++)
			p_setTextureSize(i, m_texsize);
	}


	void System::setSize(sf::Vector2f size)
	{
		m_size = size;
		m_randSize = false;

		for (size_t i = 0; i < (m_va.size() / 4); i++)
			p_setSize(i, size);
	}

	void System::setSize(sf::Vector2f minRand, sf::Vector2f maxRand)
	{
		m_size = minRand;
		m_dsize = maxRand - minRand;
		m_randSize = true;

		for (size_t i = 0; i < (m_va.size() / 4); i++)
		{
			int num = rand();
			sf::Vector2f offset;
			offset.x = num % (int)(m_dsize.x + 1) * (2 * (m_dsize.x > 0) - 1);
			offset.y = num % (int)(m_dsize.y + 1) * (2 * (m_dsize.y > 0) - 1);

			p_setSize(i, minRand + offset);
		}
	}
	
	void System::setBoundary(sf::FloatRect boundary)
	{
		m_bounds = boundary;

		cull();
	}


	void System::generate(size_t count)
	{
		size_t size = m_va.size();

		m_va.resize(size + (count * 4));

		for (size_t i = 0; i < count; i++)
			append(size / 4 + i);
	}

	void System::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_va.size())
			target.draw(m_va.data(), m_va.size(), sf::Quads, m_tex);
	}
}