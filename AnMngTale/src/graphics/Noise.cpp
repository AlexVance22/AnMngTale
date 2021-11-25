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


constexpr float Noise::grad1D(uint8_t hash, float x)
{
	/*
	switch (hash & 0xF)
	{
	case 0x0: case 0x2: case 0x4: case 0x6: case 0xC:
		return x;
	case 0x1: case 0x3: case 0x5: case 0x7: case 0xE:
		return -x;
	default:
		return 0;
	}
	
	const uint8_t h = hash & 0xF;
	const float u = x * (2 * ((h & 0x1) == 0) - 1) * (h < 0x8);
	const float v = x * (2 * ((h & 0x2) == 0) - 1) * (h == 0xC || h == 0xE);
	return u + v;
	*/

	const uint8_t h = hash & 0xF;
	const float u = x * (2 * (h & 0x1) - 1) * (h < 8);
	const float v = x * (2 * (h == 12) - (h == 12 || h == 14));
	return u + v;
}

constexpr float Noise::grad2D(uint8_t hash, float x, float y)
{
	switch (hash & 0xF)
	{
	case 0x4: case 0x6:
		return x;
	case 0x5: case 0x7:
		return -x;
	case 0x8: case 0xA:
		return y;
	case 0x9: case 0xB: case 0xD: case 0xF:
		return -y;
	case 0x0: case 0xC:
		return  x + y;
	case 0x1: case 0xE:
		return -x + y;
	case 0x2:
		return  x - y;
	case 0x3:
		return -x - y;
	default:
		return 0;
	}
}


void Noise::reseed(uint32_t seed)
{
	for (size_t i = 0; i < 256; ++i)
		p[i] = static_cast<uint8_t>(i);

	std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

	//for (size_t i = 0; i < 256; ++i)
	//	p[256 + i] = p[i];
}

float Noise::noise1D(float x) const
{
	const int32_t X = static_cast<int32_t>(x) & 0xFE;

	x -= std::floor(x);

	return lerp(grad1D(p[p[p[X]]], x), grad1D(p[p[p[X+1]]], x-1), fade(x));
}