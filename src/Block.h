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

    //colors
    int red;
    int green;
    int blue;

    Block(SDL_Rect r, int b);
    
};