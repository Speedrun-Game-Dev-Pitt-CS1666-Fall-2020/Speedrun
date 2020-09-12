#pragma once

class SimplexNoise {
public:

	int seed = 420;

	//must call updateFractalBounds after changing these variables
	float freq = 0.01f;
	int octaves = 1;
	float lacunarity = 2.f;
	float gain = .5f;

	explicit SimplexNoise(int seed) {
		setSeed(seed);
	}

	void setSeed(int seed);

	void updateFractalBounds();
	
	float getSingle(float x, float y) const;
	float getFractal(float x, float y) const;

	

private:
	unsigned char permIndex[512];//256, except we apply index wrapping to prevent a lot of modular arithmetic 
	unsigned char gradPerm[512];//now max octave is 256 w out repetition :( no infinitely zooming fractals on one seed hmmmmmm. jk oh well

	float fractalBounds = 1.f;

	

	inline unsigned char getGradIndex(unsigned char offset, int x, int y) const;
	inline float getGradAt(unsigned char offset, int x, int y, float xd, float yd) const;

	inline float single(float x, float y, unsigned char offset) const;
};