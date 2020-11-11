#include "BouncyBlock.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

BouncyBlock::BouncyBlock(float x, float y, int w, int h) : x_pos{x}, y_pos{y}, width{w}, height{h}
{
    x_vel = 2;
    y_vel = 2;
    x_accel = 0;
    //gravity
    y_accel = 1;

}

void BouncyBlock::updatePosition()
{
	//move horizontally, constrained by screen width
    x_pos += x_vel;

	//move vertically, contrained by depth of world
    y_pos += y_vel;

}

//probably will pass in array of rects that will apply force
void BouncyBlock::applyForces()
{

    //for colliding with bottom of screen
    //apply gravity. by default starts at gravity of 1

    x_vel += x_accel;

    y_vel += y_accel;


}

void BouncyBlock::detectCollisionsBlock(std::vector <Block> r)
{
    for (auto block: r)
    {
        if (isColliding(&(block.block_rect))){
            //for now also pass in a, later will get blocktype from struct
            handleCollision(block);
            //std::cout << a << std::endl;
        }
    }
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

//FOR NOW, int a gets block height for block type. will get block type from struct
void Player::handleCollision(Block r)
{
    float bounce = 15;

    if(r.block_type == 1){
        friction = 0.1;
    }

    //Check player velocity direction for

    //get the x and y constraints for box based off of velocity direction
    float requiredX = 0;
    float requiredY = 0;

    float complementaryX = 0;
    float complementaryY = 0;

    if (x_vel < 0)
    {
        //hitting collision object from right
        //need x pos of player to be box + boxwidth
        requiredX = r.block_rect.x + r.block_rect.w;
    }
    else if(x_vel > 0)
    {
        //hitting collision object from left
        //need x pos of player to be box - playerwidth
        requiredX = r.block_rect.x - width;
    }else{
        requiredX = 0;
    }

    if (y_vel < 0)
    {
        //hitting collision object from bot
        //need y pos of player to be box+boxheight
        requiredY = r.block_rect.y + r.block_rect.h;
    }
    else if(y_vel > 0)
    {
        //hitting collision object from top
        //need y pos of player to be box - playerheight
        requiredY = r.block_rect.y - height;
    }else{
        requiredY = 0;
    }

    //y2 = y1 + slope (x2 - x1)
    complementaryY = y_pos + (-y_vel / x_vel) * (requiredX - x_pos);
    complementaryX = x_pos + (x_vel / y_vel) * (requiredY - y_pos);

    if(x_vel == 0){
        x_pos = complementaryX;
        y_pos = requiredY;
        //if falling, cant jump is false
        if(y_vel > 0){
            cantJump = false;
        }

        //not bouncy
        if(r.block_type != 2){
            y_vel = 0;
        }else{  //bouncy
            if(y_vel > 0){
                y_vel = -bounce;
                cantJump = true;
            }else{
                y_vel = bounce;
            }
        }
    
        //y_vel = 0;
    
        
    }else if(y_vel == 0){
        y_pos = complementaryY;
        x_pos = requiredX;


        //not bouncy
        if(r.block_type != 2){
            x_vel = 0;
        }else{  //bouncy
            if(x_vel > 0){
                x_vel = -bounce;
            }else{
                x_vel = bounce;
            }
        }

        //x_vel = 0;

    }else {

        //which point is closer to current position?
        //dist formula
        if(powf((y_pos - complementaryY), 2) + powf((x_pos - requiredX), 2) < powf((x_pos - complementaryX), 2) + powf((y_pos - requiredY), 2)){
            //required X closer
            //y_pos = complementaryY;
            x_pos = requiredX;

            //not bouncy
            if(r.block_type != 2){
                x_vel = 0;
            }else{  //bouncy
                if(x_vel > 0){
                    x_vel = -bounce;
                }else{
                    x_vel = bounce;
                }
            }

            //x_vel = 0;

        }else{
            //x_pos = complementaryX;
            y_pos = requiredY;

            //if falling, cant jump is false
            if(y_vel > 0)
                cantJump = false;
            
            //not bouncy
            if(r.block_type != 2){
                y_vel = 0;
            }else{  //bouncy
                if(y_vel > 0){
                    y_vel = -bounce;
                    cantJump = true;
                }else{
                    y_vel = bounce;
                }
            }

            //y_vel = 0;
        }

    }

}
