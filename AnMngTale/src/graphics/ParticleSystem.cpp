#include "PCH.h"
#include "ParticleSystem.h"


void ParticleSystem::p_setPosition(size_t idx, sf::Vector2f position)
{
	const sf::Vector2f size = p_getSize(idx);

	m_va[idx * 4].position = position;

	p_setSize(idx, size);
}

sf::Vector2f ParticleSystem::p_getPosition(size_t idx) const
{
	return m_va[idx * 4].position;
}


void ParticleSystem::p_setSize(size_t idx, sf::Vector2f size)
{
	const sf::Vector2f pos = m_va[idx * 4].position;

	m_va[idx * 4 + 1].position = sf::Vector2f(pos.x + size.x, pos.y);
	m_va[idx * 4 + 2].position = pos + size;
	m_va[idx * 4 + 3].position = sf::Vector2f(pos.x, pos.y + size.y);
}

sf::Vector2f ParticleSystem::p_getSize(size_t idx) const
{
	return m_va[idx * 4 + 2].position - m_va[idx * 4 + 0].position;
}


void ParticleSystem::p_setColor(size_t idx, sf::Color color)
{
	for (size_t i = 0; i < 4; i++)
		m_va[idx * 4 + i].color = color;
}

void ParticleSystem::p_setTextureSize(size_t idx, sf::Vector2f size)
{
	m_va[idx * 4 + 0].texCoords = sf::Vector2f(0, 0);
	m_va[idx * 4 + 1].texCoords = sf::Vector2f(size.x, 0);
	m_va[idx * 4 + 2].texCoords = size;
	m_va[idx * 4 + 3].texCoords = sf::Vector2f(0, size.y);
}


void ParticleSystem::p_move(size_t idx, sf::Vector2f movement)
{
	p_setPosition(idx, p_getPosition(idx) + movement);
}

sf::FloatRect ParticleSystem::p_getBounds(size_t idx) const
{
	return sf::FloatRect(p_getPosition(idx), p_getSize(idx));
}


void ParticleSystem::p_erase(size_t idx)
{
	for (int64_t i = 3; i >= 0; i--)
		m_va.erase(m_va.begin() + (idx * 4) + i);
}


void ParticleSystem::cull()
{
	for (int64_t i = (int64_t)m_particles.size() - 1; i >= 0; i--)
	{
		if (!p_getBounds(i).intersects(m_bounds))
		{
			p_erase(i);
			m_particles.erase(m_particles.begin() + i);
		}
	}
}


void ParticleSystem::doFireflies(const float deltaTime)
{
	for (size_t i = 0; i < m_particles.size(); i++)
	{
		const sf::Vector2f raw(m_noise.noise1D(m_particles[i].noiseCoord.x * 0.005f) * 20.f,
							   m_noise.noise1D(m_particles[i].noiseCoord.y * 0.002f) * 20.f);

		const sf::Vector2f size = p_getSize(i);

		p_move(i, raw * (size.x + size.y) * 0.5f * deltaTime);

		m_particles[i].noiseCoord.x++;
		m_particles[i].noiseCoord.y++;
	}

	cull();
}

void ParticleSystem::doSnow(const float deltaTime)
{
	m_totaltime -= deltaTime;

	if (m_totaltime < 0)
	{
		append();
		m_totaltime = m_gentime;
	}

	for (size_t i = 0; i < m_particles.size(); i++)
	{
		const sf::Vector2f raw(m_noise.noise1D(m_particles[i].noiseCoord.x * 0.005f) * 15.f,
							   m_noise.noise1D(m_particles[i].noiseCoord.y * 0.002f) * 5.f + 4.f);

		const sf::Vector2f size = p_getSize(i);

		p_move(i, raw * (size.x + size.y) * 0.5f * deltaTime);

		m_particles[i].noiseCoord.x++;
		m_particles[i].noiseCoord.y++;
	}

	cull();
}


void ParticleSystem::doFountain(const float deltaTime)
{
	for (size_t i = 0; i < m_particles.size(); i++)
	{
		if (m_dragStr)
		{
			//calculate drag velocity as force of drag in opposite direction to particles velocity
			const sf::Vector2f vel = m_particles[i].velocity;
			const float lenvel = std::sqrt(vel.x * vel.x + vel.y * vel.y);
			const sf::Vector2f veldir = m_particles[i].velocity / lenvel;
			const sf::Vector2f drag = -veldir * m_dragStr;

			m_particles[i].velocity += drag * deltaTime * 60.f;
			//drag cannot change particles direction - if sign changed, multiply by 0
			m_particles[i].velocity.x *= ((m_particles[i].velocity.x < 0) == (vel.x < 0));
			m_particles[i].velocity.y *= ((m_particles[i].velocity.y < 0) == (vel.y < 0));

			/*
			if (std::abs(vel.x) < std::abs(drag.x))
				m_particles[i].velocity.x = 0;
			else
				m_particles[i].velocity.x -= drag.x;

			if (std::abs(vel.y) < std::abs(drag.y))
				m_particles[i].velocity.y = 0;
			else
				m_particles[i].velocity.y -= drag.y;
			*/
		}

		m_particles[i].velocity += m_gravityDir * m_gravityStr * deltaTime * 60.f;

		p_move(i, m_particles[i].velocity * deltaTime * 60.f);

		cull();
	}
}


ParticleSystem::ParticleSystem(sf::FloatRect rect) : m_bounds(rect), m_color(255, 255, 255)
{
	m_noise.reseed((uint32_t)std::time(nullptr));
}


void ParticleSystem::setColor(sf::Color color)
{
	m_color = color;

	for (size_t i = 0; i < m_particles.size(); i++)
		p_setColor(i, m_color);
}

void ParticleSystem::setTexture(const sf::Texture& tex)
{
	m_tex = &tex;
	m_texsize = (sf::Vector2f)tex.getSize();

	for (size_t i = 0; i < m_particles.size(); i++)
		p_setTextureSize(i, m_texsize);
}


void ParticleSystem::setSize(sf::Vector2f size)
{
	m_size = size;
	m_randSize = false;

	for (size_t i = 0; i < m_particles.size(); i++)
		p_setSize(i, size);
}

void ParticleSystem::setSize(sf::Vector2f minRand, sf::Vector2f maxRand)
{
	m_size = minRand;
	m_dsize = maxRand - minRand;
	m_randSize = true;

	for (size_t i = 0; i < m_particles.size(); i++)
	{
		const int num = rand();

		const sf::Vector2f offset(
			(float)(num % (int)(m_dsize.x + 1)) * (2 * (m_dsize.x > 0) - 1),
			(float)(num % (int)(m_dsize.y + 1)) * (2 * (m_dsize.y > 0) - 1)
		);

		p_setSize(i, minRand + offset);
	}
}


void ParticleSystem::setMode(Mode mode)
{
	m_mode = mode;
}

void ParticleSystem::setBoundary(sf::FloatRect boundary)
{
	m_bounds = boundary;

	cull();
}


void ParticleSystem::setMinParticles(uint32_t minimum)
{
	m_minParticles = minimum;
}


void ParticleSystem::setGenRate(float ratePerSec)
{
	m_gentime = 1.f / ratePerSec;
}


void ParticleSystem::setGravityStr(float force)
{
	m_gravityStr = force;
}

void ParticleSystem::setGravityDir(float degrees)
{
	const float radians = (degrees / 180.f) * 3.14159265f;

	m_gravityDir.x = std::cosf(radians);
	m_gravityDir.y = std::sinf(radians);
}

void ParticleSystem::setDragStr(float force)
{
	m_dragStr = force;
}


void ParticleSystem::append(int idx)
{
	if (m_mode != Mode::None)
	{
		if (idx == -1)
		{
			idx = (int)m_particles.size();
			m_va.resize(m_va.size() + 4);
			m_particles.emplace_back();
		}

		sf::Vector2f size = m_size;
		if (m_randSize)
		{
			const int num = rand();
			size.x += (float)(num % (int)(m_dsize.x + 1)) * (2 * (m_dsize.x > 0) - 1);
			size.y += (float)(num % (int)(m_dsize.y + 1)) * (2 * (m_dsize.y > 0) - 1);
		}

		p_setSize(idx, size);
		p_setColor(idx, m_color);
		p_setTextureSize(idx, m_texsize);

		const sf::Vector2u noiseCoord((uint32_t)idx * 263U, (uint32_t)idx * 842U + 268325U);
		const float angle = (rand() % 361) * (180.f / 3.14159265f);
		const sf::Vector2f velocity = sf::Vector2f(std::cos(angle), std::sin(angle)) * m_startVel;

		m_particles[idx] = Particle{ noiseCoord, velocity, angle };

		sf::Vector2f pos;

		switch (m_mode)
		{
		case Mode::Fireflies:
			pos.x = m_bounds.left + rand() % (int)(m_bounds.width + 1);
			pos.y = m_bounds.top + rand() % (int)(m_bounds.height + 1);
			break;
		case Mode::Snow:
			pos.x = m_bounds.left + rand() % (int)(m_bounds.width + 1);
			pos.y = m_bounds.top - size.y;
			break;
		case Mode::Fountain:
			pos = m_emitter;
			break;
		}

		p_setPosition(idx, pos);
	}
}

void ParticleSystem::generate(size_t count)
{
	if (m_mode != Mode::None)
	{
		const size_t size = m_particles.size();

		m_particles.resize(size + count);
		m_va.resize((size + count) * 4);

		for (size_t i = 0; i < count; i++)
			append((int)(size + i));
	}
}


void ParticleSystem::update(const float deltaTime)
{
	switch (m_mode)
	{
	case Mode::Fireflies:
		doFireflies(deltaTime);
		break;
	case Mode::Snow:
		doSnow(deltaTime);
		break;
	case Mode::Fountain:
		doFountain(deltaTime);
		break;
	}
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_va.size())
		target.draw(m_va.data(), m_va.size(), sf::Quads, m_tex);
}