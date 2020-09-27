#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "XorShifter.h"
#include "SimplexNoise.h"
#include "Screen.h"
#include "Image.h"
#include "Player.h"

#define CREDIT_SIZE 10
#define MENU_SIZE 2

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

//for move player tutorial, may move to player object later
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;

// Globals
Screen *screen = nullptr;
Uint32 before;
Uint32 then;
Uint32 delta;
Uint32 now;

// Function declarations
bool init();
void close();
Image *loadImage(const char *, int, int);
void runCredits();
void runGame();
void runMenu();
SDL_Texture *loadTexture(std::string);

Image *loadImage(const char *src, int w, int h)
{
	return new Image(screen, src, 0, 0, w, h);
}

SDL_Texture *loadTexture(std::string fname)
{
	SDL_Texture *newText = nullptr;

	SDL_Surface *startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr)
	{
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(screen->renderer, startSurf);
	if (newText == nullptr)
	{
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}

bool init()
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Failed to load SDL" << std::endl;
		return false;
	}
	screen = new Screen("SpeedRun", SCREEN_WIDTH, SCREEN_HEIGHT);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		std::cerr << "Failed to initialize SDL_image" << std::endl;
		return false;
	}

	then = SDL_GetTicks();

	return true;
}

void close()
{
	delete screen;
	// Quit SDL subsystems
	SDL_Quit();
}

void runCredits()
{

	XorShifter *rng = new XorShifter(412001000);
	SimplexNoise *simp = new SimplexNoise(420);
	simp->freq = 0.02f;
	simp->octaves = 2;
	simp->updateFractalBounds();

	Image *credits[CREDIT_SIZE] = {

		loadImage("../res/rjd68.png", 800, 600),
		loadImage("../res/alex.png", 1280, 720),
		loadImage("../res/andrew.png", 1280, 720),
		loadImage("../res/cas380.png", 1280, 720),
		loadImage("../res/connor.png", 1280, 720),
		loadImage("../res/jacob.png", 1280, 720),
		loadImage("../res/lucas.png", 1280, 720),
		loadImage("../res/robert.png", 1280, 720),
		loadImage("../res/spencer.png", 1280, 720),
		loadImage("../res/ryanyang.png", 1280, 720)};

	float dt;

	while (!screen->isClosed())
	{
		const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
		SDL_Event e;
		if (keystate[SDL_SCANCODE_ESCAPE] || e.type == SDL_QUIT)
			return;

		now = SDL_GetTicks();
		delta = now - then;
		if (delta >= 16)
		{
			then = now;
			dt = (float)delta / 16.f;
			//std::cout << delta << std::endl;

			SDL_SetRenderDrawColor(screen->renderer, 255, 0, 0, 255);
			SDL_RenderClear(screen->renderer);
			SDL_Rect pixel = {0, 0, 10, 10};
			for (int x = 0; x < 128; x++)
			{
				for (int y = 0; y < 72; y++)
				{
					//int grey = (int)(rng->fnext() * 256);

					Uint8 grey = (int)((simp->getFractal((float)x, (float)y, ((float)now) / 16.f)) * 256);

					SDL_SetRenderDrawColor(screen->renderer, grey, grey, grey, 255);
					pixel.x = x * 10;
					pixel.y = y * 10;
					SDL_RenderFillRect(screen->renderer, &pixel);
				}
			}
			int index = (now - before) / 3000;
			if (index >= CREDIT_SIZE)
				break;
			//std::cout << index << std::endl;
			Image *img = credits[index];
			//std::cout << img->bounds->w << std::endl;
			SDL_RenderCopy(screen->renderer, img->texture, img->bounds, screen->bounds);
			SDL_RenderPresent(screen->renderer);

			screen->pollEvents();
		}
	}
}

void runGame()
{
	// Create player object with x, y, w, h, texture
	Player *user = new Player(10, 0, 20, 20, loadTexture("../res/Guy.png"));

	SDL_Event e;
	bool gameon = true;
	while (gameon)
	{

		user->applyForces();

		//get intended motion based off input
		while (SDL_PollEvent(&e))
		{
			const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
			if (e.type == SDL_QUIT || keystate[SDL_SCANCODE_ESCAPE])
			{
				gameon = false;
			}
			else
			{

				if(keystate[SDL_SCANCODE_W]){

					if (!user->isJumping)
					{
						user->y_vel += -15;
						user->isJumping = true;
					}

				}
				if(keystate[SDL_SCANCODE_A]){
					//user->x_accel = -0.5;
					if(user->x_vel > -6){
						user->x_vel += -1;
					}
				}
				if(keystate[SDL_SCANCODE_D]){
					//user->x_accel = 0.5;
					if(user->x_vel < 6){
						user->x_vel += 1;
					}
				}
				if(keystate[SDL_SCANCODE_S]){
					
				}
			}
		}

		// Move box
		user->updatePosition();

		//check constraints and resolve conflicts
		//apply forces based off gravity and collisions
		user->detectCollisions();

		// Draw box
		// Clear black
		SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(screen->renderer);
		// Player box
		SDL_Rect player_rect = {user->x_pos, user->y_pos, user->width, user->height};
		SDL_RenderCopy(screen->renderer, user->player_texture, NULL, &player_rect);
		SDL_RenderPresent(screen->renderer);
	}
}

void runMenu()
{
	SDL_Event e;
	bool gameon = true;
	bool menuon = true;
	int menuPos = 0;

	Image *menu[MENU_SIZE] = {
		loadImage("../res/play.png", 1280, 720),
		loadImage("../res/creds.png", 1280, 720),
	};

	while (gameon)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				gameon = false;
			}
		}

		if (menuon)
		{

			const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
			if (keystate[SDL_SCANCODE_RETURN] && menuPos == 0)
			{
				runGame();
			}
			if (keystate[SDL_SCANCODE_RETURN] && menuPos == 1)
			{
				before = SDL_GetTicks();
				runCredits();
			}
			if (keystate[SDL_SCANCODE_A] && menuPos == 1)
			{
				//can go left
				menuPos = 0;
			}
			if (keystate[SDL_SCANCODE_D] && menuPos == 0)
			{
				//can go right
				menuPos = 1;
			}

			SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(screen->renderer);
			Image *img = menu[menuPos];
			SDL_RenderCopy(screen->renderer, img->texture, img->bounds, screen->bounds);
			SDL_RenderPresent(screen->renderer);
		}
	}
}

int main(int argc, char **argv)
{

	// Initialize Game
	if (!init())
	{
		std::cout << "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	runMenu();

	// Close Game
	close();

	return 0;
}