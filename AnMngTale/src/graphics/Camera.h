#pragma once


class Camera : public sf::View
{
private:
	const sf::Vector2f* m_dynamicTarget = nullptr;
	sf::Vector2f m_staticTarget;

	sf::View m_parallax;

	float m_dampSpeed = 1.0f;

	sf::Vector2f m_constraintMin;
	sf::Vector2f m_constraintMax;
	bool m_constraintEnabled = false;

public:
	using sf::View::View;

	void setDynamicTarget(const sf::Vector2f* target);
	void setStaticTarget(sf::Vector2f target);

	void setConstraints(sf::Vector2f constraintMin, sf::Vector2f constraintMax);
	void enableConstraints(bool enable)
	{
		m_constraintEnabled = enable;
	}
	void setDamping(float damping)
	{
		m_dampSpeed = 1.f / damping;
	}
	
	void update(const float deltaTime);

	const sf::View& parallax(float scale);
};