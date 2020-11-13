#pragma once

#include "Vec2.h"
#include <SDL.h>
#include "GameMap.h"
#include "XorShifter.h"

class MapGenerator {
    
public:
    GameMap* map;
    XorShifter* rng;

    int tubeLength;
    Vec2* tubePoints;
    float* tubeWidths;

    MapGenerator(GameMap* map);
    ~MapGenerator();

    void genTube();
    void voxelize();
    void genBiomes();
private:
    Vec2 tube_min;
    Vec2 tube_max;
    //void genSurface();
    //void genTube();
    //void genNoise();
};