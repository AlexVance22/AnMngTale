#include "PCH.h"
#include "Noise.h"


constexpr float Noise::fade(float t)
{
	return t * t * t * (t * (t * 6.f - 15.f) + 10.f);
}

constexpr float Noise::lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

constexpr float Noise::grad(std::uint8_t hash, float x)
{
	const uint8_t h = hash & 15;
	const float u = (h < 8) * x;
	const float v = (h == 12 || h == 14) * x * !(h < 4);
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}


void Noise::reseed(uint32_t seed)
{
	for (size_t i = 0; i < 256; ++i)
		p[i] = static_cast<uint8_t>(i);

	std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

	for (size_t i = 0; i < 256; ++i)
		p[256 + i] = p[i];
}

float Noise::noise1D(float x) const
{
	const int32_t X = static_cast<int32_t>(std::floor(x)) & 255;

	x -= std::floor(x);

	return lerp(grad(p[p[p[X]]], x), grad(p[p[p[X+1]]], x-1), fade(x));
}