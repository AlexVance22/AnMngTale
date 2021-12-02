#include "PCH.h"
#include "Camera.h"


void Camera::setDynamicTarget(const sf::Vector2f* target)
{
	m_dynamicTarget = target;
}

void Camera::setStaticTarget(sf::Vector2f target)
{
	m_dynamicTarget = nullptr;
	m_staticTarget = target;
}

void Camera::moveToTarget()
{
	if (m_dynamicTarget)
		setCenter(*m_dynamicTarget);
	else
		setCenter(m_staticTarget);
}


void Camera::setConstraints(sf::Vector2f constraintMin, sf::Vector2f constraintMax)
{
	m_constraintMin = constraintMin;
	m_constraintMax = constraintMax;
}

void Camera::enableConstraints(bool enable)
{
	m_constraintEnabled = enable;
}

void Camera::setDamping(float damping)
{
	m_dampSpeed = 1.f / damping;
}


void Camera::update(const float deltaTime)
{
	if (m_dynamicTarget)
		move((*m_dynamicTarget - getCenter()) * m_dampSpeed);
	else
		move((m_staticTarget - getCenter()) * m_dampSpeed);

	if (m_constraintEnabled)
	{
		float x = std::clamp(getCenter().x, m_constraintMin.x, m_constraintMax.x);
		float y = std::clamp(getCenter().y, m_constraintMin.y, m_constraintMax.y);

		setCenter(x, y);
	}
}


const sf::View& Camera::parallax(float scale)
{
	return m_parallax;
}