#pragma once

#include "Vec2.h"
#include <SDL.h>

class TerrainMap {
    
public: 
    char** chunks;
    int seed;
    int cw,ch,w,h;

    TerrainMap(int seed, int width, int height);
    ~TerrainMap();

    void get(int x, int y);
private:

    
};