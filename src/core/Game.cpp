#include "Game.h"
#include "StateManager.h"
#include <iostream>

namespace Game {

    Screen* screen = nullptr;
    StateManager* state_manager = nullptr;
    Cursor* cursor = nullptr;

    
    namespace time{
        int then = 0;
        int now = 0;
        int delta = 0;
        float dt = 0.f;
    }

    void start(){
        time::then = SDL_GetTicks();
        
        bool _running = true;

        while (_running)
        {
            time::now = SDL_GetTicks();
            time::delta = time::now - time::then;
            if (time::delta >= time::FRAME_DUR)
            {
                time::dt = time::delta/time::FRAME_DUR;
                time::then = time::now;

                _running = InputManager::pollEvents() && state_manager->size!=0;
                if(_running){
                    
                    SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
                    SDL_RenderClear(screen->renderer);

                    state_manager->peek()->is_active = true;
                    for(int i = 0; i < state_manager->size; i++){
                        if(state_manager->states[i]->tick())
                            delete state_manager->remove(i--);
                    }

                    SDL_RenderPresent(screen->renderer);
                }
            }
        }
        std::cout << "Exiting game loop..." << std::endl;
    }
}
