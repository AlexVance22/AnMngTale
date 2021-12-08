#pragma once

#include "core/Scene.h"


class Floor400 : public Scene
{
private:
	sf::Vector2f m_topLayerPos{ -525, -2775 };
	sf::Vector2f m_topLayerSize{ 1005, 3045 };

private:
	void loadFlags();
	void dumpFlags();

	void impl(const float deltaTime) override;

public:
	Floor400();
};