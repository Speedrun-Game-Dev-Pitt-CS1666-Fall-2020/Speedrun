#include "BouncyBlock.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

BouncyBlock::BouncyBlock(float x, float y, int w, int h, float xv, float yv) : x_pos{x}, y_pos{y}, width{w}, height{h}, x_vel{xv}, y_vel{yv}
{
    x_accel = 0;
    //gravity
    y_accel = 1;

}

/*
BouncyBlock::~BouncyBlock(){
    delete this;
}
*/

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

    if (x_vel > 10)
        x_vel = 10;

    if (x_vel < -10)
        x_vel = -10;

    if (y_vel > 15)
        y_vel = 15;

    if (y_vel < -8)
        y_vel = -8;


}

void BouncyBlock::detectCollisionsBlock(std::vector <Block> r)
{
    for (auto block: r)
    {
        if (isColliding(&(block.block_rect)) && block.block_type != 7){
            //for now also pass in a, later will get blocktype from struct
            handleCollision(block);
            //std::cout << a << std::endl;
        }
    }
}

void BouncyBlock::detectCollisionsBouncy(std::vector <BouncyBlock> r)
{
    // for (auto block: r)
    // {
    //     SDL_Rect temp = {block.x_pos, block.y_pos, block.width, block.height}
    //     if (isColliding(&(block.block_rect))){
    //         //for now also pass in a, later will get blocktype from struct
    //         handleCollision(block.block_rect);
    //         //std::cout << a << std::endl;
    //     }

    //     delete(temp);
    // }
}

bool BouncyBlock::isColliding(SDL_Rect *r)
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
void BouncyBlock::handleCollision(Block r)
{

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

        y_vel *= -1;
    
        
    }else if(y_vel == 0){
        y_pos = complementaryY;
        x_pos = requiredX;


        //not bouncy
        x_vel *= -1;



    }else {

        //which point is closer to current position?
        //dist formula
        if(powf((y_pos - complementaryY), 2) + powf((x_pos - requiredX), 2) < powf((x_pos - complementaryX), 2) + powf((y_pos - requiredY), 2)){
            //required X closer
            //y_pos = complementaryY;
            x_pos = requiredX;

            x_vel *= -1;

            //x_vel = 0;

        }else{
            //x_pos = complementaryX;
            y_pos = requiredY;

            //if falling, cant jump is false
            
            //not bouncy
            y_vel *= -1;

            //y_vel = 0;
        }

    }

}
