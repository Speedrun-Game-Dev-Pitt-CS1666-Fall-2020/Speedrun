#include "Cursor.h"
#include "InputManager.h"
#include <iostream>

void Cursor::start(){
    this->isLeftClick = false;
}

void Cursor::handle_event(SDL_Event e){
    int cx = 0;
    int cy = 0;
    switch(e.type){
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&cx, &cy);
            screen_pos.x = (float)cx;
            screen_pos.y = (float)cy;
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch(e.button.button){
                case SDL_BUTTON_LEFT:
                    this->isLeftDown = true;
                    break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch(e.button.button){
                case SDL_BUTTON_LEFT:
                    this->isLeftDown = false;
                    this->isLeftClick = true;
                    break;
            }
            break;
    }
}