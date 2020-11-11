#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

class BouncyBlock
{

public:
    float x_pos;
    float y_pos;
    float x_vel;
    float y_vel;
    float y_accel;
    int width;
    int height;

    BouncyBlock(float x, float y, int w, int h);
    void updatePosition();

    void applyForces();

    void detectCollisionsBlocks(std::vector <Block> r);
    void detectCollisionsBouncy(std::vector <BouncyBlock> r);
    bool isColliding(SDL_Rect* r);
    void handleCollision(SDL_Rect r);
    
};