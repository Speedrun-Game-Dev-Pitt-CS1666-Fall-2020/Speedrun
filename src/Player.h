#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Player {

public:

    int x_pos;
    int y_pos;
    int x_vel;
    int y_vel;
    int x_accel;
    int y_accel;

    int width;
    int height;

    bool isJumping;

    SDL_Texture* player_texture;

    Player(int x, int y, int w, int h, SDL_Texture* t);
    void updatePosition();

    void applyForces();

    void detectCollisions();
    bool isColliding(SDL_Rect r);

};