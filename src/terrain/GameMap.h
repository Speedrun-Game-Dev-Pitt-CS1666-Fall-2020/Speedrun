#pragma once

#include "Vec2.h"
#include <SDL.h>

class GameMap {

    
public: 
    unsigned char** field;
    unsigned char** mat_field;
    int seed;
    int w,h;

    GameMap(int seed);
    ~GameMap();

    char getDensity(Vec2 pos);
private:

};