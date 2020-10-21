#include "XorShifter.h"
#include "TerrainGenerator.h"
#include "SimplexNoise.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <SDL.h>

namespace MapGenerator {

}
/*
#define TAU 6.2831853071795f
#define PI 3.14159265358979f
#define INFLUENCE 0.05f//scale of 1-2

TerrainGenerator::TerrainGenerator(int seed, int w, int h){
    this->seed = seed;
    this->width = w;
    this->height = h;

    this->genTube();
}

void TerrainGenerator::genTube(){
    Vec2 pos = Vec2(1280/2,0);
    float velRad = PI;//start going right
    float strength = 4.f;
    float t = 0.f;

    XorShifter* rng = new XorShifter(89);
    SimplexNoise* noise_x = new SimplexNoise(rng->next());
    SimplexNoise* noise_y = new SimplexNoise(rng->next());
    noise_x->freq = 0.01f;
    noise_y->freq = 0.01f;

    float min = 1000000.f;
    float max = -100000.f;
    float avg = 0;
    for(int i = 0; i < 1000; i++){
        this->tubePoints.push_back(pos);

        float dx = noise_x->getSingle(pos.x,pos.y,t);
        float dy = noise_y->getSingle(pos.x,pos.y,t);

        t+=0.1f;
        
        dx*=10.f;
        dx-=5.f;
        dy/=2.f;
        dy-=0.1f;
        //if( std::abs(d-velRad) > PI)d+=TAU;
        //velRad += d;//(velRad+(INFLUENCE*d))/(1.f+INFLUENCE);
        //velRad = fmodf(velRad,TAU);
        std::cout << velRad << '\n';
        /*
        if(velRad<PI){
            if(velRad>PI/2)velRad = PI;
            else velRad=0;
        }*

        Vec2 velFrame = Vec2(strength*dx,strength*dy);
        pos.x += velFrame.x;
        pos.y += velFrame.y;
        //pos.y += 0.5f;
        //std::cout << velRad << '\n'
        //    << std::cos(velRad) << '\n';
    }
    std::cout << "min: " << min << '\n'
        << "max: " << max << '\n';

    delete noise_x;
    delete noise_y;
    delete rng;
}

SDL_Point* TerrainGenerator::getDebugTubePoints(){

    int size = this->tubePoints.size();

    SDL_Point* points = new SDL_Point[size];

    for(int i = 0; i < size; i++){
        points[i].x = this->tubePoints[i].x;
        points[i].y = this->tubePoints[i].y;
    }

    return points;
}
*/