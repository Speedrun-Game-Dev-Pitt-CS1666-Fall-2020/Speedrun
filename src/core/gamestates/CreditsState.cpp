#include "CreditsState.h"
#include "Game.h"
#include <iostream>
#include "SimplexNoise.h"
#include "SpriteLib.h"
#include "Renderer.h"

CreditsState::CreditsState(){
    time_elapsed = 0;

    closeBtn = new Button("X", 32, 10, 10, 50, 50);

    simp = new SimplexNoise(420);
	simp->freq = 0.02f;
	simp->octaves = 2;
	simp->updateFractalBounds();
}

bool CreditsState::tick() {

    time_elapsed += Game::time::delta;

    SDL_Rect pixel = {0, 0, 10, 10};
    for (int x = 0; x < 128; x++)
    {
        for (int y = 0; y < 72; y++)
        {
            //int grey = (int)(rng->fnext() * 256);
            
            Uint8 grey = (int)((simp->getFractal((float)x, (float)y, ((float)time_elapsed) / 16.f)) * 256);

            SDL_SetRenderDrawColor(Game::screen->renderer, grey, grey, grey, 255);
            pixel.x = x * 10;
            pixel.y = y * 10;
            SDL_RenderFillRect(Game::screen->renderer, &pixel);
        }
    }
    int index = time_elapsed / 3000;
    if (index >= 10){
        return true;
    }
    //std::cout << index << std::endl;
    Renderer::draw_fullscreen(SpriteLib::CREDITS[index]);
    //std::cout << img->bounds->w << std::endl;

    if(closeBtn->tick()){
        return true;
    }

    return false;
}

CreditsState::~CreditsState(){
    delete simp;
    delete closeBtn;
}