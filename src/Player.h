#pragma once
#include <SDL.h>
#include <SDL_image.h>

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

    int width;
    int height;

    bool cantJump;

    SDL_Texture *player_texture;

    Player(float x, float y, int w, int h, SDL_Texture *t);
    void updatePosition();

    void applyForces();

    void detectCollisions(SDL_Rect* r,  SDL_Rect* r2);
    bool isColliding(SDL_Rect* r);
    void handleCollision(SDL_Rect* r);
    
};