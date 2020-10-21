#pragma once

#include "Vec2.h"
#include <SDL.h>

class TerrainMap {
    
public: 
    char**field;
    int seed;
    int width;
    int height;

    TerrainMap(int seed, int width, int height);

private:

    void genSurface();
    void genTube();
    void genNoise();
};