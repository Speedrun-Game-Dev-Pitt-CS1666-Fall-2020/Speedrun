
#include "InputManager.h"
#include<SDL.h>
#include <iostream>

namespace InputManager {
    int event_size = 0;
    SDL_Event* events = new SDL_Event[MAX_EVENT_SIZE];

    bool pollEvents(){
        event_size = 0;
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return false;
                default:
                    events[event_size++] = event;
                    break;
            }
        }
        return true;
    }
}

