#pragma once


class Noise
{
private:
	uint8_t p[512] = { 0 };

public:
	constexpr static float fade(float t);
	constexpr static float lerp(float a, float b, float t);
	constexpr static float grad(uint8_t hash, float x);

	void reseed(uint32_t seed);
	float noise1D(float x) const;
};