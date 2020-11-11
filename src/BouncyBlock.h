#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Block.h"
#include <vector>

class BouncyBlock
{

public:
    float x_pos;
    float y_pos;
    float x_vel;
    float y_vel;
    float x_accel;
    float y_accel;
    int width;
    int height;

    BouncyBlock(float x, float y, int w, int h, float xv, float yv);
    void updatePosition();

    void applyForces();

    void detectCollisionsBlock(std::vector <Block> r);
    void detectCollisionsBouncy(std::vector <BouncyBlock> r);
    bool isColliding(SDL_Rect* r);
    void handleCollision(Block r);
    
};