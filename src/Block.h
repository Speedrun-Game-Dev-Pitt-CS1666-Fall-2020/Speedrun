#pragma once
#include <SDL.h>

#include <SDL_image.h>
#include <vector>

class Block
{

public:
    SDL_Rect block_rect;
    //0 = normal, 1 = frictionless, 2 = bouncy, 3 = win
    int block_type;

    bool moving;
    float speed; 
    float time_period;

    //colors
    int red;
    int green;
    int blue;

    float time;

    Block(SDL_Rect r, int b, bool m, float s, int t);
    void updatePosition();
};