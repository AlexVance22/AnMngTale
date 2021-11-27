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
	const uint8_t h = hash & 0xF;
	const float u = x * (h < 8) * (2 * (h & 0x1) - 1);
	const float v = x * ((h == 12) - (h == 14));
	return u + v;
	*/

	const uint8_t h = hash % 12;
	return x * (h < 8) * (2 * (h & 0x1) - 1);
}

constexpr float Noise::grad2D(uint8_t hash, float x, float y)
{
	/*
	const uint8_t h = hash & 0xF;
	const uint8_t flip = 2 * (h & 0x1) - 1;
	const float u = x * flip * (h < 8) + y * (2 * (h < 2) - (h < 4));
	const float v = y * flip * (h > 7) + x * ((h == 12) - (h == 14));
	return u + v;
	*/

	const uint8_t h = hash & 12;
	const uint8_t flip = 2 * (h & 0x1) - 1;
	return x * (h < 8) * flip + y * ((h > 7) * flip + (2 * (h < 2) - (h < 4)));
}

constexpr float Noise::grad3D(uint8_t hash, float x, float y, float z)
{
	switch (hash % 12)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	default: return 0;
	}

	/*
	const uint8_t h = hash & 0xF;
	const float u = h < 8 ? x : y;
	const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	*/

	/*
	const uint8_t h = hash & 12;
	const uint8_t flip = 2 * (h & 0x1) - 1;
	return x * (h < 8) * flip + y * ((h > 7) * flip + (2 * (h < 2) - (h < 4))) + z * (h > 3) * (2 * (h & 0x2) - 1);
	*/
}


Noise::Noise()
{
	for (uint32_t i = 0; i < 256; i++)
		p[i] = static_cast<uint8_t>(i);
}

Noise::Noise(uint32_t seed)
{
	for (uint32_t i = 0; i < 256; i++)
		p[i] = static_cast<uint8_t>(i);

	reseed(seed);
}


void Noise::reseed(uint32_t seed)
{
	std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

	for (uint32_t i = 0; i < 256; i++)
		p[256 + i] = static_cast<uint8_t>(i);
}


float Noise::noise1D(float x) const
{
	const int32_t X = static_cast<int32_t>(x) & 0xFF;

	x -= std::floor(x);

	float u = fade(x);

	int aaa = p[p[p[X]]];
	int baa = p[p[p[X+1]]];

	return lerp(grad1D(aaa, x), grad1D(baa, x-1), u);
}

float Noise::noise2D(float x, float y) const
{
	const int32_t X = static_cast<int32_t>(x) & 0xFF;
	const int32_t Y = static_cast<int32_t>(y) & 0xFF;

	x -= std::floor(x);
	y -= std::floor(y);

	float u = fade(x);
	float v = fade(y);

	int aaa = p[p[p[X]		+ Y]];
	int aba = p[p[p[X]		+ Y+1]];
	int baa = p[p[p[X+1]	+ Y]];
	int bba = p[p[p[X+1]	+ Y+1]];

	return lerp(lerp(grad2D(aaa, x, y), grad2D(baa, x-1, y), u),
				lerp(grad2D(aba, x, y-1), grad2D(bba, x-1, y-1), u),
				v);
}

float Noise::noise3D(float x, float y, float z) const
{
	const int32_t X = static_cast<int32_t>(x) & 0xFF;
	const int32_t Y = static_cast<int32_t>(y) & 0xFF;
	const int32_t Z = static_cast<int32_t>(z) & 0xFF;

	x -= std::floor(x);
	y -= std::floor(y);
	z -= std::floor(z);

	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	int aaa = p[p[p[X]		+ Y]	+ Z];
	int aba = p[p[p[X]		+ Y+1]	+ Z];
	int aab = p[p[p[X]		+ Y]	+ Z+1];
	int abb = p[p[p[X]		+ Y+1]	+ Z+1];
	int baa = p[p[p[X+1]	+ Y]	+ Z];
	int bba = p[p[p[X+1]	+ Y+1]	+ Z];
	int bab = p[p[p[X+1]	+ Y]	+ Z+1];
	int bbb = p[p[p[X+1]	+ Y+1]	+ Z+1];

	return lerp(lerp(lerp(grad3D(aaa, x, y, z), grad3D(baa, x-1, y, z), u),
					 lerp(grad3D(aba, x, y-1, z), grad3D(bba, x-1, y-1, z), u),
					 v),
				lerp(lerp(grad3D(aab, x, y, z-1), grad3D(bab, x-1, y, z-1), u),
					 lerp(grad3D(abb, x, y-1, z-1), grad3D(bbb, x-1, y-1, z-1), u),
					 v),
				w);
}