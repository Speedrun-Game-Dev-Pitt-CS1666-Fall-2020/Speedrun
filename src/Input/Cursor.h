#pragma once

#include "Vec2.h"
#include <SDL.h>

class Cursor {
    
    public:

        Vec2 screen_pos;
        Vec2 game_pos;
        bool isLeftClick;
        bool isLeftDown;

        void start();
        void handle_event(SDL_Event e);
};