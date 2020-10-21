#pragma once

#include <SDL.h>
#include "Camera.h"
#include "Screen.h"
#include "StateManager.h"
#include "InputManager.h"
#include "Cursor.h"



namespace Game{
    

    extern Screen* screen;
    extern StateManager* state_manager;
    extern Cursor* cursor;

    
    namespace time{
        constexpr int FPS = 60;
        constexpr int FRAME_DUR = 1000 / FPS;
        extern int then;
        extern int now;
        extern int delta;
        extern float dt;
    }

    extern void start();

    
}