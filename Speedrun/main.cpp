#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "XorShifter.h"
#include "SimplexNoise.h"
#include "Screen.h"

static const int WIDTH = 1280;
static const int HEIGHT = 720;

Screen* screen;


int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to load SDL" << std::endl;
	}
	screen = new Screen("Durk does SDL2!", WIDTH, HEIGHT);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cerr << "Failed to initialize SDL_image" << std::endl;
	}

	XorShifter* rng = new XorShifter(412001000);
	SimplexNoise* simp = new SimplexNoise(420);
	simp->freq = 0.02f;
	simp->octaves = 3;
	simp->updateFractalBounds();



	auto surface = IMG_Load("res/rjd68.png");
	if (!surface) {
		std::cerr << "Failed to create surface.\n";
	}
	auto texture = SDL_CreateTextureFromSurface(screen->renderer, surface);
	if (!texture) {
		std::cerr << "Failed to create texture.\n";
	}
	SDL_Rect textureBounds = { 0,0,800,600 };
	SDL_Rect screenBounds = { 0,0,WIDTH,HEIGHT };


	Uint32 then = SDL_GetTicks();
	Uint32 delta;
	Uint32 now;
	float dt;
	while (!screen->isClosed()) {
		now = SDL_GetTicks();
		delta = now - then;
		if (delta >= 16) {
			then = now;
			dt = (float)delta / 16.f;
			std::cout << delta << std::endl;

			SDL_SetRenderDrawColor(screen->renderer, 255, 0, 0, 255);
			SDL_RenderClear(screen->renderer);
			SDL_Rect pixel = { 0, 0, 10, 10 };
			for (int x = 0; x < 128; x++) {
				for (int y = 0; y < 72; y++) {
					//int grey = (int)(rng->fnext() * 256);

					Uint8 grey = (int)((simp->getFractal((float)x, (float)y, ((float)now)/16.f)) * 256);
					
					SDL_SetRenderDrawColor(screen->renderer, grey, grey, grey, 255);
					pixel.x = x * 10;
					pixel.y = y * 10;
					SDL_RenderFillRect(screen->renderer, &pixel);
					
				}
			}

			SDL_RenderCopy(screen->renderer, texture, &textureBounds, screen->dimensions);
			SDL_RenderPresent(screen->renderer);

			screen->pollEvents();
		}
	}


	delete screen;
	return 0;
}