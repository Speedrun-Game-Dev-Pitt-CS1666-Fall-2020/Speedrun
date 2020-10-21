#pragma once

#include<SDL.h>

namespace InputManager {
    constexpr int MAX_EVENT_SIZE = 256;//way overkill probably
    extern int event_size;
    extern SDL_Event* events;

    extern bool pollEvents();

}