#include "Player.h"

Player::Player(int x, int y, int w, int h, SDL_Texture* t) :
 x_pos{x}, y_pos{y}, width{w}, height{h}, player_texture{t} {
     x_vel = 0;
     y_vel = 0;
     x_accel = 0;
     y_accel = 1;
 }

void Player::updatePosition() {
    x_pos += x_vel;
    y_pos += y_vel;
}