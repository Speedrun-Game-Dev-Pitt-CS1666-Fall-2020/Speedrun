

#include "GameMap.h"

GameMap::GameMap(int seed){
    this->seed = seed;
}

GameMap::~GameMap(){
    for(int i = 0; i < w; i++){
        delete field[i];
        delete mat_field[i];
    }
    delete mat_field;
    delete field;
}

char GameMap::getDensity(Vec2 pos){

}