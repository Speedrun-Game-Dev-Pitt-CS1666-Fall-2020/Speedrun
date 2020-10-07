#include "StateManager.h"

#include <stdlib.h>
#include <iostream>
#include "GameState.h"

StateManager::StateManager(int cap){
    states = new GameState*[cap];
    this->cap = cap;
}

StateManager::~StateManager(){
    delete states;
}

void StateManager::push(GameState* gs){
    if(size==cap){
        std::cerr << "game-state stack underflow exception. exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    states[size++] = gs;
}

GameState* StateManager::pop(){
    if(size==0){
        std::cerr << "game-state stack underflow exception. exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    return states[--size];
}
GameState* StateManager::remove(int i){
    if(i < -1 || i >= size){
        std::cerr << "game-state removal index out of bounds exception. exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    GameState* ret = states[i];
    size--;
    for(; i < size; i++){
        states[i] = states[i+1];
    }

    return ret;
}

GameState* StateManager::peek(){
    if(size==0){
        std::cerr << "game-state empty stack exception. exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    return states[size-1];
}