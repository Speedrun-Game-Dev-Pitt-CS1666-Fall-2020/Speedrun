#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "XorShifter.h"
#include "SimplexNoise.h"
#include "Screen.h"
#include "Image.h"

#define CREDIT_SIZE 10


constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

//for move player tutorial, may move to player object later
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;

// Globals
Screen* screen = nullptr;

// Function declarations
bool init();
void close();
Image* loadImage(const char*, int, int);
void runCredits();
void runGame();


Image* loadImage(const char* src, int w, int h) {
	return new Image(screen, src, 0, 0, w, h);
}

bool init() {	

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to load SDL" << std::endl;
		return false;
	}
	screen = new Screen("SpeedRun", SCREEN_WIDTH, SCREEN_HEIGHT);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cerr << "Failed to initialize SDL_image" << std::endl;
		return false;
	}
			
	return true;
}

void close() {
	delete screen;
	// Quit SDL subsystems
	SDL_Quit();
}

void runCredits(){

	XorShifter* rng = new XorShifter(412001000);
	SimplexNoise* simp = new SimplexNoise(420);
	simp->freq = 0.02f;
	simp->octaves = 2;
	simp->updateFractalBounds();

	Image* credits[CREDIT_SIZE] = { 

		loadImage("../res/rjd68.png",800,600),
		loadImage("../res/alex.png",1280,720),
		loadImage("../res/andrew.png",1280,720),
		loadImage("../res/cas380.png",1280,720),
		loadImage("../res/connor.png",1280,720),
		loadImage("../res/jacob.png",1280,720),
		loadImage("../res/lucas.png",1280,720),
		loadImage("../res/robert.png",1280,720),
		loadImage("../res/spencer.png",1280,720),
		loadImage("../res/ryanyang.png",1280,720)
	};


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
			//std::cout << delta << std::endl;

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
			int index = now / 3000;
			if (index >= CREDIT_SIZE)break;
			//std::cout << index << std::endl;
			Image* img = credits[index];
			//std::cout << img->bounds->w << std::endl;
			SDL_RenderCopy(screen->renderer, img->texture, img->bounds, screen->bounds);
			SDL_RenderPresent(screen->renderer);

			screen->pollEvents();
		}
	}


}

void runGame(){

	// Current position to render the box
	// Start off with it in the middle
	int x_pos = SCREEN_WIDTH/2 - BOX_WIDTH/2;
	int y_pos = SCREEN_HEIGHT/2 - BOX_HEIGHT/2;

	// Current velocity of the box
	// Start off at reset
	int x_vel = 0;
	int y_vel = 0;

	SDL_Event e;
	bool gameon = true;
	while(gameon) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				gameon = false;
			}
			else if(e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym) {
					case SDLK_w:
						y_vel = -1;
						break;

					case SDLK_a:
						x_vel = -1;
						break;

					case SDLK_s:
						y_vel = 1;
						break;

					case SDLK_d:
						x_vel = 1;
						break;

					default:
						x_vel = 0;
						y_vel = 0;
						break;
				}
			}
			
		}

		// Move box
		x_pos += x_vel;
		y_pos += y_vel;
		
		// Draw box
		// Clear black
		SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(screen->renderer);
		// Cyan box
		SDL_SetRenderDrawColor(screen->renderer, 0x00, 0xFF, 0xFF, 0xFF);
		SDL_Rect fillRect = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};
		SDL_RenderFillRect(screen->renderer, &fillRect);
		SDL_RenderPresent(screen->renderer);
	}

}

int main(int argc, char** argv) {

	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}


	runGame();

	runCredits();

/*
	XorShifter* rng = new XorShifter(412001000);
	SimplexNoise* simp = new SimplexNoise(420);
	simp->freq = 0.02f;
	simp->octaves = 2;
	simp->updateFractalBounds();

	Image* credits[CREDIT_SIZE] = { 
		loadImage("../res/rjd68.png",800,600),
		loadImage("../res/alex.png",1280,720),
		loadImage("../res/andrew.png",1280,720),
		loadImage("../res/cas380.png",1280,720),
		loadImage("../res/connor.png",1280,720),
		loadImage("../res/jacob.png",1280,720),
		loadImage("../res/lucas.png",1280,720),
		loadImage("../res/robert.png",1280,720),
		loadImage("../res/spencer.png",1280,720),
		loadImage("../res/ryanyang.png",1280,720)
	};


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
			//std::cout << delta << std::endl;

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
			int index = now / 3000;
			if (index >= CREDIT_SIZE)break;
			//std::cout << index << std::endl;
			Image* img = credits[index];
			//std::cout << img->bounds->w << std::endl;
			SDL_RenderCopy(screen->renderer, img->texture, img->bounds, screen->bounds);
			SDL_RenderPresent(screen->renderer);

			screen->pollEvents();
		}
	}
*/

	close();
	return 0;
}