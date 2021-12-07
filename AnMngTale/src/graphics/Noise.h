#pragma once

#include <stdint.h>


class Noise
{
private:
	uint8_t p[512] = { 0 };

public:
	constexpr static float fade(float t);
	constexpr static float lerp(float a, float b, float t);

	constexpr static float grad1D(uint8_t hash, float x);
	constexpr static float grad2D(uint8_t hash, float x, float y);
	constexpr static float grad3D(uint8_t hash, float x, float y, float z);

public:
	Noise();
	Noise(uint32_t seed);

	void reseed(uint32_t seed);

	float noise1D(float x) const;
	float noise2D(float x, float y) const;
	float noise3D(float x, float y, float z) const;
};