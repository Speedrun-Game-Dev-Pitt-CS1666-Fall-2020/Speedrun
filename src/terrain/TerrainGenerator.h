#pragma once

#include "Vec2.h"
#include <SDL.h>

class TerrainGenerator {
    
public: 
    char**field;
    int seed;
    int width;
    int height;

    TerrainGenerator(int seed, int width, int height);

    SDL_Point* getDebugTubePoints();

private:

    SDL_Point* tubePoints;

    void genSurface();
    void genTube();
    void genNoise();
};