#pragma once

#include "graphics/Sprite.h"


class Entity : public sf::Drawable
{
protected:
	static const float s_unitScalar;
	static const float s_physScale;
	static const float s_invScale;

	Sprite m_sprite;
	sf::Vector2f m_position;
	sf::Vector2f m_size;

	b2Body* m_body = nullptr;
	sf::Vector2f m_colliderOffset;
	sf::Vector2f m_colliderSize;

	bool m_locked = false;
	float m_speed = 4.f;

#ifdef MNG_DEBUG
	sf::RectangleShape m_debugCollider;
#endif

public:
	Entity();

	void simulate(b2Body* body, sf::FloatRect collider);
	sf::FloatRect getCollider() const;
	void handlePhysics();

	void move(sf::Vector2f direction, const float deltaTime);
	void lock(bool locked);

	void setPosition(sf::Vector2f position);
	const sf::Vector2f& getPosition() const;
	void setSize(sf::Vector2f size);
	const sf::Vector2f& getSize() const;

	virtual void update(const float deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	friend class Scene;
	friend class Script;
};