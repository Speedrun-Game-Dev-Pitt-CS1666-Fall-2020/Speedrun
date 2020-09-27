#include "Player.h"

Player::Player(float x, float y, int w, int h, SDL_Texture *t) : x_pos{x}, y_pos{y}, width{w}, height{h}, player_texture{t}
{
    x_vel = 0;
    y_vel = 0;
    x_accel = 0;
    //gravity
    y_accel = 1;
    isJumping = false;
}

void Player::updatePosition()
{
    x_pos += x_vel;
    y_pos += y_vel;
}

//probably will pass in array of rects that will apply force
void Player::applyForces()
{

    //for colliding with bottom of screen
    //apply gravity. by default starts at gravity of 1

    x_vel += x_accel;
    y_vel += y_accel;
}

void Player::detectCollisions()
{

    //go thru all possible collision things and checkCollision/apply force
    //for colliding with bottom of screen
    if (y_pos >= 720 - height)
    {
        y_pos = 720 - height;
        //y_vel = 0;
        //y_accel = 0;
        if(y_accel > 0){
            y_accel = 0;
        }
        isJumping = false;
    }
}