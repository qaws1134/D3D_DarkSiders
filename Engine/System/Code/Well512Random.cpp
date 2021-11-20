#include "Well512Random.h"

USING(Engine)
IMPLEMENT_SINGLETON(Well512Random)


Engine::Well512Random::Well512Random(void)
{
}
Engine::Well512Random::~Well512Random(void)
{
	Free();
}

void Well512Random::RandomInit(unsigned int nSeed)
{
	index = 0;
	unsigned int s = nSeed;
	for (int i = 0; i < 16; i++)
	{
		state[i] = s;
		s += s + 73;
	}
}
_uint Well512Random::Next(int minValue, int maxValue)
{
	return (unsigned int)((Next() % (maxValue - minValue)) + minValue);
}
_uint Well512Random::Next(unsigned int maxValue)
{
	return Next() % maxValue;
}

_uint Well512Random::Next()
{
	unsigned int a, b, c, d;
	a = state[index];
	c = state[(index + 13) & 15];
	b = a ^ c ^ (a << 16) ^ (c << 15);
	c = state[(index + 9) & 15];
	c ^= (c >> 11);
	a = state[index] = b ^ c;
	d = a ^ ((a << 5) & 0xda442d24U);
	index = (index + 15) & 15;
	a = state[index];
	state[index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);
	return state[index];
}

void Engine::Well512Random::Free(void)
{
}
