<<<<<<< HEAD
#include "Player.h"#include <cmath>Player::Player(float x, float y, int w, int h, SDL_Texture *t) : x_pos{x}, y_pos{y}, width{w}, height{h}, player_texture{t}{    x_vel = 0;    y_vel = 0;    x_accel = 0;    //gravity    y_accel = 1;    cantJump = true;    friction = .3;}void Player::updatePosition(){	//move horizontally, constrained by screen width    x_pos += x_vel;	if (x_pos  < 0 || (x_pos  + 20 > 1280))//100 = tile size, 1280 = screen width			x_pos  -= x_vel;	//move vertically, contrained by depth of world    y_pos += y_vel;	if (y_pos < 0 || (y_pos + 20 > 720)) //100 = tile size, 3000 = world depth 			y_pos -= y_vel;}//probably will pass in array of rects that will apply forcevoid Player::applyForces(){    //for colliding with bottom of screen    //apply gravity. by default starts at gravity of 1    x_vel += x_accel;        if (x_vel > 0)    {        if (x_vel - friction < 0)        {            x_vel = 0;        }        else        {            x_vel -= friction;        }            }    else if (x_vel < 0)     {        if (x_vel + friction > 0)        {            x_vel = 0;        }        else        {            x_vel += friction;        }    }    if (x_vel > 4)        x_vel = 4;    if (x_vel < -4)        x_vel = -4;    y_vel += y_accel;}void Player::detectCollisions(std::vector <SDL_Rect> r){    //cant jump unless detech collision with some floor    cantJump = true;    //go thru all possible collision things and checkCollision/apply force    //for colliding with bottom of screen    if (y_pos >= 3000 - height)    {        y_pos = 3000 - height;        //y_vel = 0;        //y_accel = 0;        if (y_vel > 0)        {            y_vel = 0;        }        cantJump = false;    }    for (auto block: r)    {        if (isColliding(&block))            handleCollision(&block);    }    // if (isColliding(r))    //     handleCollision(r);    // if (isColliding(r2))    //     handleCollision(r2);}bool Player::isColliding(SDL_Rect *r){    // Check vertical overlap    if (y_pos + height <= r->y)        return false;    if (y_pos >= r->y + r->h)        return false;    // Check horizontal overlap    if (x_pos >= r->x + r->w)        return false;    if (x_pos + width <= r->x)        return false;    // Must overlap in both    return true;}//assuming detect collisions will never change player velocity variables//spring traps + friction and stuff change accel variablesvoid Player::handleCollision(SDL_Rect *r){    //Check player velocity direction for    //get the x and y constraints for box based off of velocity direction    float requiredX = 0;    float requiredY = 0;    if (x_vel < 0)    {        //hitting collision object from right        //need x pos of player to be box + boxwidth        requiredX = r->x + r->w;    }    else    {        //hitting collision object from left        //need x pos of player to be box - playerwidth        requiredX = r->x - width;    }    if (y_vel < 0)    {        //hitting collision object from bot        //need y pos of player to be box+boxheight        requiredY = r->y + r->h;    }    else    {        //hitting collision object from top        //need y pos of player to be box - playerheight        requiredY = r->y - height;        //moved on top of object        cantJump = false;    }    //what is closer? complementary X to x_pos or complementary y to y_pos?    if (fabs(y_pos - requiredY) < fabs(x_pos - requiredX))    {        //x_pos = complementaryX;        y_pos = requiredY;        y_vel = 0;        if (r->w == 100)        {            y_vel += -30;            cantJump = true;        }    }    else    {        //use requiredX bc compY closer        x_pos = requiredX;        //y_pos = complementaryY;        x_vel = 0;        x_accel = 0;    }    //if moving towards collisions in either x or y, set velocity in that direction to 0}
=======
#include "Player.h"
#include <cmath>

Player::Player(float x, float y, int w, int h, SDL_Texture *t) : x_pos{x}, y_pos{y}, width{w}, height{h}, player_texture{t}
{
    x_vel = 0;
    y_vel = 0;
    x_accel = 0;
    //gravity
    y_accel = 1;
    cantJump = true;
    friction = .3;
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
    
    if (x_vel > 0)
    {
        if (x_vel - friction < 0)
        {
            x_vel = 0;
        }
        else
        {
            x_vel -= friction;
        }
        
    }
    else if (x_vel < 0) 
    {
        if (x_vel + friction > 0)
        {
            x_vel = 0;
        }
        else
        {
            x_vel += friction;
        }
    }

    if (x_vel > 4)
        x_vel = 4;

    if (x_vel < -4)
        x_vel = -4;

    y_vel += y_accel;


}

void Player::detectCollisions(std::vector <SDL_Rect> r)
{

    //cant jump unless detech collision with some floor
    cantJump = true;

    //go thru all possible collision things and checkCollision/apply force
    //for colliding with bottom of screen
    if (y_pos >= 720 - height)
    {
        y_pos = 720 - height;
        //y_vel = 0;
        //y_accel = 0;
        if (y_vel > 0)
        {
            y_vel = 0;
        }
        cantJump = false;
    }

    for (auto block: r)
    {
        if (isColliding(&block))
            handleCollision(&block);
    }
    // if (isColliding(r))
    //     handleCollision(r);
    // if (isColliding(r2))
    //     handleCollision(r2);
}

bool Player::isColliding(SDL_Rect *r)
{
    // Check vertical overlap
    if (y_pos + height <= r->y)
        return false;
    if (y_pos >= r->y + r->h)
        return false;

    // Check horizontal overlap
    if (x_pos >= r->x + r->w)
        return false;
    if (x_pos + width <= r->x)
        return false;

    // Must overlap in both
    return true;
}

//assuming detect collisions will never change player velocity variables
//spring traps + friction and stuff change accel variables
void Player::handleCollision(SDL_Rect *r)
{
    //Check player velocity direction for

    //get the x and y constraints for box based off of velocity direction
    float requiredX = 0;
    float requiredY = 0;

    if (x_vel < 0)
    {
        //hitting collision object from right
        //need x pos of player to be box + boxwidth
        requiredX = r->x + r->w;
    }
    else
    {
        //hitting collision object from left
        //need x pos of player to be box - playerwidth
        requiredX = r->x - width;
    }

    if (y_vel < 0)
    {
        //hitting collision object from bot
        //need y pos of player to be box+boxheight
        requiredY = r->y + r->h;
    }
    else
    {
        //hitting collision object from top
        //need y pos of player to be box - playerheight
        requiredY = r->y - height;
        //moved on top of object
        cantJump = false;
    }

    //what is closer? complementary X to x_pos or complementary y to y_pos?
    if (fabs(y_pos - requiredY) < fabs(x_pos - requiredX))
    {
        //x_pos = complementaryX;
        y_pos = requiredY;
        y_vel = 0;

        if (r->w == 100)
        {
            y_vel += -30;
            cantJump = true;
        }
    }
    else
    {
        //use requiredX bc compY closer
        x_pos = requiredX;
        //y_pos = complementaryY;
        x_vel = 0;
        x_accel = 0;
    }

    //if moving towards collisions in either x or y, set velocity in that direction to 0
}
>>>>>>> upstream/master
