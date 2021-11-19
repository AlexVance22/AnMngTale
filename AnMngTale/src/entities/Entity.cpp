#include "PCH.h"
#include "Entity.h"


const float Entity::s_unitScalar = 1.f / (float)sqrt(2);

const float Entity::s_invScale = 30.f;
const float Entity::s_physScale = 1.f / Entity::s_invScale;


Entity::Entity()
{
#ifdef _DEBUG
	m_debugCollider.setFillColor(sf::Color(0, 0, 0, 0));
	m_debugCollider.setOutlineColor(sf::Color(255, 255, 255));
	m_debugCollider.setOutlineThickness(0.3f);
#endif
}


void Entity::setSprite(const sf::Texture& texture, sf::Vector2u spriteSheetSize, float framerate)
{
	m_sprite.setTexture(texture);
	m_sprite.animate(spriteSheetSize.x, spriteSheetSize.y);
	m_sprite.setFrameRate(framerate);
	m_sprite.setLoop(true);
	m_sprite.setIsPlaying(true);
}

void Entity::setAnimation(uint32_t anim)
{
	m_sprite.setAnimation(anim);
}

sf::FloatRect Entity::getTextureBounds()
{
	return m_sprite.getGlobalBounds();
}


void Entity::simulate(b2Body* body, sf::FloatRect collider)
{
	m_body = body;

	m_colliderOffset.x = collider.left;
	m_colliderOffset.y = collider.top;
	m_colliderSize.x = collider.width;
	m_colliderSize.y = collider.height;

#ifdef _DEBUG
	m_debugCollider.setSize(m_colliderSize);
#endif
}

void Entity::handlePhysics()
{
	if (m_body)
	{
		setPosition((sf::Vector2f(m_body->GetPosition().x, m_body->GetPosition().y) * s_invScale) - m_colliderOffset);

#ifdef _DEBUG
		m_debugCollider.setPosition(getPosition() + m_colliderOffset - m_colliderSize * 0.5f);
#endif
	}
}

sf::FloatRect Entity::getCollider() const
{
	return sf::FloatRect(m_position.x + m_colliderOffset.x - m_colliderSize.x * 0.5f,
						 m_position.y + m_colliderOffset.y - m_colliderSize.y * 0.5f,
						 m_colliderSize.x, m_colliderSize.y);
}


void Entity::move(sf::Vector2f direction, const float deltaTime)
{
	if (direction.x || direction.y)
	{
		if (m_body)
		{
			sf::Vector2f movement = direction * m_speed * deltaTime * 60.f;
			m_body->SetLinearVelocity(b2Vec2(movement.x, movement.y));
		}
		else
		{
			m_position += direction * m_speed * deltaTime * 60.f;
			m_sprite.setPosition(m_position);
		}

		if (direction.x)
		{
			if (direction.x > 0)
				m_sprite.setAnimation(0);
			else
				m_sprite.setAnimation(2);
		}
		if (direction.y)
		{
			if (direction.y > 0)
				m_sprite.setAnimation(1);
			else
				m_sprite.setAnimation(3);
		}
	}
	else
	{
		m_sprite.setAnimation(4);

		if (m_body)
			m_body->SetLinearVelocity(b2Vec2(0, 0));
	}
}


void Entity::setPosition(sf::Vector2f position)
{
	m_position = position;
	m_sprite.setPosition(position);

	if (m_body)
	{
		m_body->SetTransform(b2Vec2((position.x + m_colliderOffset.x) * s_physScale, (position.y + m_colliderOffset.y) * s_physScale), 0);

#ifdef _DEBUG
		m_debugCollider.setPosition(getPosition() + m_colliderOffset - m_colliderSize * 0.5f);
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


void Entity::lock(bool locked)
{
	m_locked = locked;
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