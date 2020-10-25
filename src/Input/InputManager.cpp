
#include "InputManager.h"
#include <SDL.h>
#include <iostream>
#include "Game.h"

namespace InputManager {
    bool pollEvents(){

        Game::cursor->start();

        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)return false;
            
            Game::cursor->handle_event(event);
        }
        
        return true;
    }
}
