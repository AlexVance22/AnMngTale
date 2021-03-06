#pragma once

#include "graphics/Sprite.h"


class Entity : public sf::Drawable
{
protected:
	static const float s_unitScalar;
	float m_physScale = 1.f;
	float m_invScale = 1.f;

	Sprite m_sprite;
	sf::Vector2f m_position;
	sf::Vector2f m_size;

	b2Body* m_body = nullptr;
	sf::Vector2f m_colliderOffset;
	sf::Vector2f m_colliderSize;

	bool m_locked = false;
	bool m_lockY = false;
	float m_speed = 3.f;

#ifdef MNG_DEBUG
	sf::RectangleShape m_debugCollider;
#endif

public:
	Entity();

	void simulate(b2Body* body, sf::Vector2f offset, sf::Vector2f boxSize);
	sf::FloatRect getCollider() const;
	void handlePhysics();
	void set1D(bool dim);

	void move(sf::Vector2f direction, const float deltaTime, bool overridePhysics = false);
	void lock(bool locked);
	bool isLocked() const;

	void setPosition(sf::Vector2f position);
	const sf::Vector2f& getPosition() const;
	void setSize(sf::Vector2f size);
	const sf::Vector2f& getSize() const;

	virtual void update(const float deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	friend class Deserialiser;
	friend class Script;
};