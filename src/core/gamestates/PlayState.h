#pragma once

#include "GameState.h"
#include "Button.h"
#include "SimplexNoise.h"

class PlayState : public GameState {

public:

    PlayState();
    ~PlayState();

    bool tick() override;

private:
    int time_elapsed;
    Button* closeBtn;
};