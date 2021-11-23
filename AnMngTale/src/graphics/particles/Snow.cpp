#include "PCH.h"
#include "Snow.h"


namespace particles
{
	void Snow::cull()
	{
		for (int64_t i = ((int64_t)m_va.size() / 4) - 1; i >= 0; i--)
		{
			if (!p_getBounds(i).intersects(m_bounds))
			{
				p_erase(i);
				m_noiseCoords.erase(m_noiseCoords.begin() + i);
			}
		}
	}


	Snow::Snow()
	{
		m_noise.reseed((uint32_t)std::time(nullptr));
	}

	Snow::Snow(sf::FloatRect rect, size_t count) : System(rect)
	{
		m_noise.reseed((uint32_t)std::time(nullptr));

		generate(count);
	}


	void Snow ::setGenRate(float ratePerSec)
	{
		m_gentime = 1.f / ratePerSec;
	}


	void Snow::append(int idx)
	{
		idx = (int)emplace(idx);

		sf::Vector2f pos(m_bounds.left, m_bounds.top - p_getSize(idx).y);
		pos.x += rand() % (int)(m_bounds.width + 1);

		p_setPosition(idx, pos);

		m_noiseCoords.emplace_back((uint32_t)idx * 263U, (uint32_t)idx * 842U + 268325U);
	}

	void Snow::update(const float deltaTime)
	{
		m_totaltime -= deltaTime;

		if (m_totaltime < 0)
		{
			append();
			m_totaltime = m_gentime;
		}

		for (size_t i = 0; i < m_noiseCoords.size(); i++)
		{
			const sf::Vector2f raw((float)m_noise.noise1D(m_noiseCoords[i].x * 0.005) * 15.f,
								   (float)m_noise.noise1D(m_noiseCoords[i].y * 0.002) * 5.f + 4.f);

			const sf::Vector2f size = p_getSize(i);

			p_move(i, raw * (size.x + size.y) * 0.5f * deltaTime);

			m_noiseCoords[i].x++;
			m_noiseCoords[i].y++;
		}

		cull();
	}
}