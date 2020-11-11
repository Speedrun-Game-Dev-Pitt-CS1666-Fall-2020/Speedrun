#pragma once
class XorShifter
{
public:
	explicit XorShifter(unsigned int seed) {
		x = seed;
		y = Y;
		z = Z;
		w = W;
	}

	int next();
	float fnext();


private:
	const unsigned int Y = 4001002;
	const unsigned int Z = 6002007;
	const unsigned int W = 2006003009;
	unsigned int x, y, z, w;

};

