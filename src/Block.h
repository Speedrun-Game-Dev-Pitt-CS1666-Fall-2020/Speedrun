#pragma once
#include <SDL.h>
#include <iostream>


class Block {
	
public:
	SDL_Rect* block_rect;
    bool is_bouncy;
    bool is_frictionless;
    bool is_winning;
    short color_r;
    short color_g;
    short color_b;

    Block(SDL_Rect* b, bool ib, bool f, bool iw, short cr, short cg, short cb);

};