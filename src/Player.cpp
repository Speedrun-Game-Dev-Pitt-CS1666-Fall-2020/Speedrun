#include "Player.h"
#include "Block.h"
#include "BouncyBlock.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

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

    if (y_vel > 18)
        y_vel = 18;

    if (y_vel < -18)
        y_vel = -18;

    


    //if (cantJump)
    y_vel += y_accel;


}

void Player::detectCollisions(std::vector <Block> r)
{

    //cant jump unless detech collision with some floor
    cantJump = true;

    //go thru all possible collision things and checkCollision/apply force
    //for colliding with bottom of screen
    // if (y_pos >= 3000 - height)
    // {
    //     y_pos = 3000 - height;
    //     //y_vel = 0;
    //     //y_accel = 0;
    //     if (y_vel > 0)
    //     {
    //         y_vel = 0;
    //     }
    //     cantJump = false;
    // }

    //make friction .3 which gets updated if hit frictionless block to less
    friction = 0.3;


    for (auto block: r)
    {
        if (isColliding(&(block.block_rect)) && block.block_type != 7){
            //for now also pass in a, later will get blocktype from struct
            handleCollision(block);
            //std::cout << a << std::endl;
        }
    }
}

void Player::detectBouncyBlockCollisions(std::vector <BouncyBlock> r)
{

    for (auto block: r)
    {
        
        if (isBouncyBlockColliding(block)){
            //for now also pass in a, later will get blocktype from struct
            handleBouncyBlockCollision(block);
            //std::cout << a << std::endl;
        }
        
    }
}

bool Player::isBouncyBlockColliding(BouncyBlock b)
{
    // Check vertical overlap
    if (y_pos + height <= b.y_pos)
        return false;
    if (y_pos >= b.y_pos + b.height)
        return false;

    // Check horizontal overlap
    if (x_pos >= b.x_pos + b.width)
        return false;
    if (x_pos + width <= b.x_pos)
        return false;

    // Must overlap in both
    return true;
}

void Player::handleBouncyBlockCollision(BouncyBlock r)
{

    if(r.x_vel < 0){
        x_vel += -8;
        if(x_vel < -4){
            x_vel = -4;
        }
        //std::cout<<"left"<<std::endl;
    }else if(r.x_vel > 0){
        x_vel += 8;
        if(x_vel > 4){
            x_vel = 4;
        }
        //std::cout<<"right"<<std::endl;
    }

    if(r.y_vel < 0){
        y_vel += -8;
        if(y_vel < -18){
            y_vel = -18;
        }
        //std::cout<<"up"<<std::endl;
    }else if(r.y_vel > 0){
        y_vel += 8;
        if(y_vel > 18){
            y_vel = 18;
        }
        //std::cout<<"down"<<std::endl;
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

    if(r.block_type == 3){
        std::cout << "You win!" << std::endl;
    }

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
