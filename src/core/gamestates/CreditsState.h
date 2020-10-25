#pragma once

#include "GameState.h"
#include "Button.h"
#include "SimplexNoise.h"

class CreditsState : public GameState {

public:

    CreditsState();
    ~CreditsState();

    bool tick() override;

private:
    int time_elapsed;
    Button* closeBtn;
    SimplexNoise* simp;
};