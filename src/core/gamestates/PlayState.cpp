#include "PlayState.h"
#include "Game.h"
#include <iostream>
#include "Renderer.h"
#include "MenuState.h"

PlayState::PlayState(){
    closeBtn = new Button("X", 32, 10, 10, 50, 50);
    
}

bool PlayState::tick() {



    if(closeBtn->tick()){
        Game::state_manager->push(new MenuState());
        return true;
    }
    return false;
}

PlayState::~PlayState(){
    delete closeBtn;
}