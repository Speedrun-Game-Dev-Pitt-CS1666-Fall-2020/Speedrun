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
	x_screenPos = x_pos;
	y_screenPos = y_pos;
}

void Player::updatePosition()
{
	//move horizontally, constrained by screen width
    x_pos += x_vel;

	//move vertically, contrained by depth of world
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

<<<<<<< Updated upstream
    if (cantJump)
        y_vel += y_accel;
=======
    y_vel += y_accel;
>>>>>>> Stashed changes


}

void Player::detectCollisions(std::vector <SDL_Rect> r)
{

    //cant jump unless detech collision with some floor
    cantJump = true;

    //go thru all possible collision things and checkCollision/apply force
    //for colliding with bottom of screen
    if (y_pos >= 3000 - height)
    {
        y_pos = 3000 - height;
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
<<<<<<< Updated upstream
=======
    // if (isColliding(r))
    //     handleCollision(r);
    // if (isColliding(r2))
    //     handleCollision(r2);
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    float requiredY = 0;

=======
    float complementaryY;
    float requiredY = 0;
    float complementaryX;

    //point and slope
    //point is x_pos, y_pos
    //slope is velocity, y_vel/x_vel
    //box gives y or x

    if (x_vel < 0)
    {
        //hitting collision object from right
        //need x pos of player to be box + boxwidth
        requiredX = r->x + r->w;
    }
    else if(x_vel > 0)
    {
        //hitting collision object from left
        //need x pos of player to be box - playerwidth
        requiredX = r->x - width;
    }else{
        requiredX = NAN;
    }

    if (y_vel < 0)
    {
        //hitting collision object from bot
        //need y pos of player to be box+boxheight
        requiredY = r->y + r->h;
    }
    else if(y_vel > 0)
    {
        //hitting collision object from top
        //need y pos of player to be box - playerheight
        requiredY = r->y - height;
        //moved on top of object
        //cantJump = false;
    }else{
        requiredY = NAN;
    }

    //solve for complementary x and complementary y
    //y2 = y1 + slope (x2 - x1)
    //doesn't work when x_vel == 0

    complementaryY = y_pos + (y_vel / x_vel) * (requiredX - x_pos);

    //doesn't work when y_vel == 0
    complementaryX = x_pos + (x_vel / y_vel) * (requiredY - y_pos);

    /*
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======
    */
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
}
=======
}
>>>>>>> Stashed changes
