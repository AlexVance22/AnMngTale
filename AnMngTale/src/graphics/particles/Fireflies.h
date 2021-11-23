#pragma once

#include "System.h"


namespace particles
{
	class Fireflies : public System
	{
	private:
		std::vector<sf::Vector2u> m_noiseCoords;
		siv::PerlinNoise m_noise;

	private:
		void cull() override;

	public:
		Fireflies();
		Fireflies(sf::FloatRect rect, size_t count);

		void append(int idx = -1) override;
		void update(const float deltaTime) override;
	};
}