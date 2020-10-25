#pragma once

class GameState {
    public:
        bool is_active;

        GameState(){is_active=true;}
        ~GameState(){}

        virtual bool tick() = 0;
};