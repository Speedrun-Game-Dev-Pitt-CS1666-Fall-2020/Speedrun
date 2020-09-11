#include "SimplexNoise.h"
#include "XorShifter.h"
#include <iostream>
#include <random>

const float GRAD_X[12] = {
	1,-1,1,-1,
	1,-1,1,-1,
	0,0,0,0
};

const float GRAD_Y[12] = {
	1,1,-1,-1,
	0,0,0,0,
	1,-1,1,-1
};


//shuffle permutation array
void SimplexNoise::setSeed(int seed)
{
	this->seed = seed;
	
	XorShifter* rng = new XorShifter(seed);
	//std::mt19937_64 gen(seed);

	for (int i = 0; i < 256; i++)permIndex[i] = i;

	for (int i = 0; i < 256; i++)
	{
		int rand = i+(rng->next() % (256 - i));
		int j = permIndex[i];
		permIndex[i] = permIndex[i + 256] = permIndex[rand];
		permIndex[rand] = j;
		gradPerm[i] = gradPerm[i + 256] = permIndex[i] % 12;
	}
}

void SimplexNoise::updateFractalBounds() {
	float amp = gain;
	float ampFractal = 1.f;
	for (int i = 1; i < octaves; i++)
	{
		ampFractal += amp;
		amp *= gain;
	}
	fractalBounds = 1.f / ampFractal;
}

inline unsigned char SimplexNoise::getGradIndex(unsigned char offset, int x, int y) const {
	return gradPerm[(x & 0xff) + permIndex[(y & 0xff) + offset]];
}

inline float SimplexNoise::getGradAt(unsigned char offset, int ix, int iy, float x, float y) const {
	unsigned char i = getGradIndex(offset, ix, iy);
	return x * GRAD_X[i] + y * GRAD_Y[i];
}


static const float ROOT3 = 1.7320508075688772935274463415059f;
static const float SKEW = 0.5f * (ROOT3 - 1.f); //orthogonal to simplex
static const float UNSKEW = (3.f - ROOT3) / 6.f; //simplex to orthogonal
static const float UNSKEW2 = (3.f - ROOT3) / 3.f; //simplex to orthogonal

inline int fastFloor(float n) {
	return (n >= 0 ? (int)n : (int)n-1);
}

inline float SimplexNoise::single(float x, float y, unsigned char offset) const {
	float t = (x + y) * SKEW;//hairy factor
	int i = fastFloor(x + t);
	int j = fastFloor(y + t);

	t = (i + j) * UNSKEW;
	float x0 = x - (i - t);
	float y0 = y - (j - t);

	int i1 = x0 > y0 ? 1 : 0;//branchless fun
	int j1 = i1 ? 0 : 1;

	float x1 = x0 - (float)i1 + UNSKEW;
	float y1 = y0 - (float)j1 + UNSKEW;
	float x2 = x0 - 1.f + UNSKEW2;
	float y2 = y0 - 1.f + UNSKEW2;

	float n0, n1, n2;

	//6t^5-15t^4+10t^3
	t = 0.5f - x0 * x0 - y0 * y0;
	if (t < 0) {
		n0 = 0;
	}
	else
	{
		t *= t;
		n0 = t * t * getGradAt(offset, i, j, x0, y0);
	}

	t = 0.5f - x1 * x1 - y1 * y1;
	if (t < 0) {
		n1 = 0;
	}
	else
	{
		t *= t;
		n1 = t * t * getGradAt(offset, i + i1, j + j1, x1, y1);
	}

	t = 0.5f - x2 * x2 - y2 * y2;
	if (t < 0) n2 = 0;
	else
	{
		t *= t;
		n2 = t * t * getGradAt(offset, i + 1, j + 1, x2, y2);
	}
	//std::cout << (35.f * (n0 + n1 + n2)) + 0.5f << std::endl;
	return (35.f * (n0 + n1 + n2))+0.5f;
}

float SimplexNoise::getSingle(float x, float y) const {
	return single(x*freq,y*freq,0);//no need for extra fast seed offset
}

float SimplexNoise::getFractal(float x, float y) const{//this is why permIndex array
	x *= freq;
	y *= freq;
	float sum = single(x,y,permIndex[0]);
	float amp = 1;

	for (int i = 1; i < octaves; i++) {
		x *= lacunarity;
		y *= lacunarity;

		amp *= gain;
		sum += single(x,y,permIndex[i])*amp;
	}
	return sum * fractalBounds;
}

