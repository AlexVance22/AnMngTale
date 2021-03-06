#include "PCH.h"
#include "Entity.h"


const float Entity::s_unitScalar = 1.f / (float)sqrt(2);


Entity::Entity()
{
#ifdef _DEBUG
	m_debugCollider.setFillColor(sf::Color(0, 0, 0, 0));
	m_debugCollider.setOutlineColor(sf::Color(255, 255, 255));
	m_debugCollider.setOutlineThickness(0.3f);
#endif
}


void Entity::simulate(b2Body* body, sf::Vector2f offset, sf::Vector2f boxSize)
{
	m_body = body;

	m_colliderOffset = offset;
	m_colliderSize = boxSize;

#ifdef _DEBUG
	m_debugCollider.setSize(m_colliderSize);
#endif
}

sf::FloatRect Entity::getCollider() const
{
	return sf::FloatRect(m_position + m_colliderOffset - m_colliderSize * 0.5f, m_colliderSize);
}

void Entity::handlePhysics()
{
	if (m_body)
	{
		m_position = sf::Vector2f(m_body->GetPosition().x, m_body->GetPosition().y) * m_invScale - m_colliderOffset;
		m_sprite.setPosition(m_position);

#ifdef _DEBUG
		m_debugCollider.setPosition(m_position + m_colliderOffset - m_colliderSize * 0.5f);
#endif
	}
}

void Entity::set1D(bool dim)
{
	m_lockY = dim;
}


void Entity::move(sf::Vector2f direction, const float deltaTime, bool overridePhysics)
{
	if (direction.x || direction.y)
	{
		direction.y *= !m_lockY;

		if (m_body && !overridePhysics)
		{
			sf::Vector2f movement = direction * m_speed;
			m_body->SetLinearVelocity(b2Vec2(movement.x, movement.y));
		}
		else
			setPosition(getPosition() + direction * m_speed * deltaTime * 60.f * 0.5f);

		if (m_sprite.isAnimated())
		{
			if (direction.x > 0)
				m_sprite.setAnimation(0);
			else if (direction.x < 0)
				m_sprite.setAnimation(2 - m_lockY);

			if (direction.y > 0)
				m_sprite.setAnimation(1);
			else if (direction.y < 0)
				m_sprite.setAnimation(3);
		}
	}
	else
	{
		if (m_sprite.isAnimated())
			m_sprite.setAnimation(4 - 2 * m_lockY);

		if (m_body && !overridePhysics)
			m_body->SetLinearVelocity(b2Vec2(0, 0));
	}
}

void Entity::lock(bool locked)
{
	m_locked = locked;

	if (m_body)
		m_body->SetLinearVelocity(b2Vec2(0, 0));
}

bool Entity::isLocked() const
{
	return m_locked;
}


void Entity::setPosition(sf::Vector2f position)
{
	m_position = position;
	m_sprite.setPosition(position);

	if (m_body)
	{
		m_body->SetTransform(b2Vec2((position.x + m_colliderOffset.x) * m_physScale, (position.y + m_colliderOffset.y) * m_physScale), 0);

#ifdef _DEBUG
		m_debugCollider.setPosition(m_position + m_colliderOffset - m_colliderSize * 0.5f);
#endif
	}
}

const sf::Vector2f& Entity::getPosition() const
{
	return m_position;
}

void Entity::setSize(sf::Vector2f size)
{
	m_size = size;
}

const sf::Vector2f& Entity::getSize() const
{
	return m_size;
}


void Entity::update(const float deltaTime)
{
	m_sprite.update(deltaTime);
	handlePhysics();
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite);

#ifdef _DEBUG
	target.draw(m_debugCollider);
#endif
}