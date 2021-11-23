#pragma once

#include "System.h"


namespace particles
{
	class Snow : public System
	{
	private:
		std::vector<sf::Vector2u> m_noiseCoords;
		siv::PerlinNoise m_noise;
		float m_totaltime = 0.f;
		float m_gentime = 1.f;

	private:
		void cull() override;

	public:
		Snow();
		Snow(sf::FloatRect rect, size_t count);

		void setGenRate(float ratePerSec);

		void append(int idx = -1) override;
		void update(const float deltaTime) override;
	};
}