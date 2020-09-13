#include "SDL.h"
#include <iostream>;
#include "XorShifter.h"
#include "SimplexNoise.h"

static const int WIDTH = 1280;
static const int HEIGHT = 720;

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	XorShifter* rng = new XorShifter(412001000);
	SimplexNoise* simp = new SimplexNoise(123);



	simp->octaves = 10;
	simp->updateFractalBounds();
	std::cout << "Hello" << std::endl;

	for (int x = 0; x < 1280; x++) {
		for (int y = 0; y < 720; y++) {
			//int grey = (int)(rng->fnext() * 256);
			
			Uint8 grey = (int)((simp->getFractal((float)x,(float)y))*256);
			
			SDL_SetRenderDrawColor(renderer, grey, grey, grey, 255);
			SDL_RenderDrawPoint(renderer,x,y);
		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);


	SDL_RenderPresent(renderer);
	SDL_Delay(10000);

	

	return 0;
}