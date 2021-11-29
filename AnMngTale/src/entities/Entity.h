#pragma once

#include "graphics/Sprite.h"


class Entity : public sf::Drawable
{
protected:
	static const float s_unitScalar;

	Sprite m_sprite;
	sf::Vector2f m_position;
	sf::Vector2f m_size;

	static const float s_physScale;
	static const float s_invScale;
	b2Body* m_body = nullptr;
	sf::Vector2f m_colliderOffset;
	sf::Vector2f m_colliderSize;

	sf::RectangleShape m_debugCollider;

	bool m_locked = false;
	float m_speed = 4.f;

public:
	Entity();

	void simulate(b2Body* body, sf::FloatRect collider);
	void handlePhysics();
	sf::FloatRect getCollider() const;

	void move(sf::Vector2f direction, const float deltaTime);

	void setPosition(sf::Vector2f position);
	const sf::Vector2f& getPosition() const;
	void setSize(sf::Vector2f size);
	const sf::Vector2f& getSize() const;

	void lock(bool locked);

	virtual void update(const float deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	friend class Scene;
	friend class Script;
};