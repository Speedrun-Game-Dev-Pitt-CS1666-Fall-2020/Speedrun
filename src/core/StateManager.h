#pragma once

#include "GameState.h"

class StateManager {
public:

    GameState** states;
    int size = 0;

    StateManager(int cap);
    ~StateManager();

    void push(GameState* gs);

    GameState* pop();
    GameState* remove(int i);

    GameState* peek();


private:
    int cap;
};