#include "PCH.h"
#include "Player.h"


Player::Player()
{
	m_speed = 4.f;
}


void Player::update(const float deltaTime)
{
	m_sprite.update(deltaTime);

	if (!m_locked)
	{
		sf::Vector2f direction;
		direction.x -= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		direction.x += sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		direction.y -= sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		direction.y += sf::Keyboard::isKeyPressed(sf::Keyboard::S);

		if (direction.x && direction.y)
			direction *= s_unitScalar;

		move(direction, deltaTime);
		handlePhysics();
	}
}