#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Block.h"
#include "BouncyBlock.h"

class Player
{

public:
    float x_pos;
    float y_pos;
    float x_vel;
    float y_vel;
    float x_accel;
    float y_accel;
    float friction;
	float x_screenPos;
	float y_screenPos;
    int width;
    int height;

    bool cantJump;

    SDL_Texture *player_texture;

    Player(float x, float y, int w, int h, SDL_Texture *t);
    void updatePosition();

    void applyForces();

    void detectCollisions(std::vector <Block> r);
    bool isColliding(SDL_Rect* r);
    void handleCollision(Block r);

    void detectBouncyBlockCollisions(std::vector <BouncyBlock> r);
    bool isBouncyBlockColliding(BouncyBlock b);
    void handleBouncyBlockCollision(BouncyBlock r);
    
};