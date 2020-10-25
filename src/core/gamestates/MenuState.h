#pragma once

#include "GameState.h"
#include <iostream>
#include "Button.h"

class MenuState : public GameState {

public:

    MenuState();
    ~MenuState();

    bool tick() override;

private:
    
    Button* playBtn;
    Button* creditsBtn;
    Button* optionsBtn;

};