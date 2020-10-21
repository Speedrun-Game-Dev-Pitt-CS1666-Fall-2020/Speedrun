#include "MenuState.h"
#include "Game.h"
#include "CreditsState.h"


MenuState::MenuState(){
    playBtn = new Button("Play", 64, 50, 445, 475, 100);

    optionsBtn = new Button("Options", 32, 50, 570, 225, 100);

    creditsBtn = new Button("Credits", 32, 300, 570, 225, 100);
}

bool MenuState::tick() {
    if(playBtn->tick() && is_active){

        std::cout << "I clicked play!" << std::endl;

    }
    if(optionsBtn->tick() && is_active){
        std::cout << "I clicked options!" << std::endl;
    }
    if(creditsBtn->tick() && is_active){
        is_active = false;
        Game::state_manager->push(new CreditsState());
    }

    return false;
}

MenuState::~MenuState(){
    delete playBtn;
    delete optionsBtn;
    delete creditsBtn;
}