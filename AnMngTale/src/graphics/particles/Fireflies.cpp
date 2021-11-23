#include "PCH.h"
#include "Fireflies.h"


namespace particles
{
	void Fireflies::cull()
	{
		for (int64_t i = ((int64_t)m_va.size() / 4) - 1; i >= 0; i--)
		{
			if (p_getBounds(i).intersects(m_bounds))
			{
				p_erase(i);
				m_noiseCoords.erase(m_noiseCoords.begin() + i);
			}
		}
	}


	Fireflies::Fireflies()
	{
		m_noise.reseed((uint32_t)std::time(nullptr));
	}

	Fireflies::Fireflies(sf::FloatRect rect, size_t count) : System(rect)
	{
		m_noise.reseed((uint32_t)std::time(nullptr));

		for (size_t i = 0; i < count; i++)
			append();
	}


	void Fireflies::append(int idx)
	{
		idx = emplace(idx);

		sf::Vector2f pos(m_bounds.left, m_bounds.top);
		pos += sf::Vector2f(rand() % ((int)m_bounds.width + 1), rand() % ((int)m_bounds.height + 1));

		p_setPosition(idx, pos);

		m_noiseCoords.emplace_back((uint32_t)idx * 263U, (uint32_t)idx * 842U + 268325U);
	}

	void Fireflies::update(const float deltaTime)
	{
		for (size_t i = 0; i < m_noiseCoords.size(); i++)
		{
			sf::Vector2f raw((float)m_noise.noise1D(m_noiseCoords[i].x * 0.1),
							 (float)m_noise.noise1D(m_noiseCoords[i].y * 0.04));

			p_move(i, raw * p_getSize(i).x * deltaTime);

			m_noiseCoords[i].x += 1;//(int)(60.f * deltaTime);
			m_noiseCoords[i].y += 1;//(int)(60.f * deltaTime);
		}

		cull();
	}
}