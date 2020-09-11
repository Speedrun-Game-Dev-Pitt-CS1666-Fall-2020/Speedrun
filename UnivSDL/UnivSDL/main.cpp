#include "SDL.h"
#include <iostream>;

int main(int argc, char* argv[]) {
	std::cout << "Starting window..." << std::endl;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Credits Roll", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	std::cout << "Window opened!" << std::endl;

	SDL_Delay(3000);

	std::cout << "Ending process..." << std::endl;

	return 0;
}