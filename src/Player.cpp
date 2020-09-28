#include "Player.h"

Player::Player(float x, float y, int w, int h, SDL_Texture *t) : x_pos{x}, y_pos{y}, width{w}, height{h}, player_texture{t}
{
    x_vel = 0;
    y_vel = 0;
    x_accel = 0;
    //gravity
    y_accel = 1;
    isJumping = true;
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

void Player::detectCollisions(SDL_Rect* r)
{

    //go thru all possible collision things and checkCollision/apply force
    //for colliding with bottom of screen
    if (y_pos >= 720 - height)
    {
        y_pos = 720 - height;
        //y_vel = 0;
        //y_accel = 0;
        if(y_vel > 0){
            y_vel = 0;
        }
        isJumping = false;
    }

    if (isColliding(r)) 
        handleCollision(r);
}

bool Player::isColliding(SDL_Rect* r) {
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

void Player::handleCollision(SDL_Rect* r) {
    //Check player velocity direction for 
}