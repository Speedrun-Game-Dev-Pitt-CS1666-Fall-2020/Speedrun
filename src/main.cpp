#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "XorShifter.h"
#include "SimplexNoise.h"
#include "Screen.h"
#include "Image.h"

#define CREDIT_SIZE 10
#define MENU_SIZE 2

static const int WIDTH = 1280;
static const int HEIGHT = 720;

Screen* screen;


Image* loadImage(const char* src, int w, int h) {
	return new Image(screen, src, 0, 0, w, h);
}

int main(int argc, char** argv) {
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
	simp->octaves = 2;
	simp->updateFractalBounds();

	Image* credits[CREDIT_SIZE] = {
		loadImage("./res/rjd68.png",800,600),
		loadImage("./res/alex.png",1280,720),
		loadImage("./res/andrew.png",1280,720),
		loadImage("./res/cas380.png",1280,720),
		loadImage("./res/connor.png",1280,720),
		loadImage("./res/jacob.png",1280,720),
		loadImage("./res/lucas.png",1280,720),
		loadImage("./res/robert.png",1280,720),
		loadImage("./res/spencer.png",1280,720),
		loadImage("./res/ryanyang.png",1280,720)
	};

	Image* menu[MENU_SIZE] = {
		loadImage("./res/play.png",1280,720),
		loadImage("./res/creds.png",1280,720),
	};


	Uint32 then = SDL_GetTicks();
	Uint32 delta;
	Uint32 now;
	Uint32 before = SDL_GetTicks();
	float dt;

	SDL_Event e;
	bool gameon = true;
	bool menuon = true;
	int menuPos = 0;

	while(gameon){
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				gameon = false;
			}
		}

		std::cout << before << std::endl;

		if(menuon){

			const Uint8* keystate = SDL_GetKeyboardState(nullptr);
			if(keystate[SDL_SCANCODE_RETURN] && menuPos == 0){
				//menuon = false;
			}
			if(keystate[SDL_SCANCODE_RETURN] && menuPos == 1){
				menuon = false;
				before = SDL_GetTicks();
			}
			if(keystate[SDL_SCANCODE_A] && menuPos == 1){
				//can go left
				menuPos = 0;
			}
			if(keystate[SDL_SCANCODE_D] && menuPos == 0){
				//can go right
				menuPos = 1;
			}

			SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(screen->renderer);
			Image* img = menu[menuPos];
			SDL_RenderCopy(screen->renderer, img->texture, img->bounds, screen->bounds);
			SDL_RenderPresent(screen->renderer);
		}else{

			const Uint8* keystate = SDL_GetKeyboardState(nullptr);
			if(keystate[SDL_SCANCODE_ESCAPE])
				menuon = true;
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

				int index = (now - before) / 3000;
				if (index >= CREDIT_SIZE)
				{
					menuon = true;
				}
				//std::cout << index << std::endl;
				Image* img = credits[index];
				//std::cout << img->bounds->w << std::endl;
				SDL_RenderCopy(screen->renderer, img->texture, img->bounds, screen->bounds);
				SDL_RenderPresent(screen->renderer);

				screen->pollEvents();
			}
		}
	}


	delete screen;
	return 0;
}
