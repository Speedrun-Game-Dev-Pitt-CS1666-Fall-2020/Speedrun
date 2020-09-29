#include <iostream>
#include <vector>
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

constexpr int WORLD_DEPTH = 3000;
constexpr int TILE_SIZE = 100;
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
SDL_Texture* loadTexture(std::string fname);
void runCredits();
void runGame();
void runMenu();
void generateTerrain();
//globals
SDL_Texture* gBackground;
SDL_Texture* dude;

Image *loadImage(const char *src, int w, int h)
{
	return new Image(screen, src, 0, 0, w, h);
}
SDL_Texture* loadTexture(std::string fname) {
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(screen->renderer, startSurf);
	if (newText == nullptr) {
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
	//my stuff
	//generateTerrain();
	gBackground = loadTexture("../res/background.png");
	int scroll_offset = 0;
	int rem = 0;
	SDL_Rect moveBox = {SCREEN_WIDTH/2 - TILE_SIZE/2, SCREEN_HEIGHT/2 - TILE_SIZE/2, TILE_SIZE, TILE_SIZE};	

	
	int tthird = (SCREEN_HEIGHT / 3) - TILE_SIZE/2;
	int bthird = (2 * SCREEN_HEIGHT/ 3) - TILE_SIZE/2;
	
	SDL_Rect playercam = {SCREEN_WIDTH/2 - TILE_SIZE/2, SCREEN_HEIGHT/2 - TILE_SIZE/2, TILE_SIZE, TILE_SIZE};
	SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	
	// Create player object with x, y, w, h, texture
	Player *user = new Player(10, 0, 100, 100, loadTexture("../res/Guy.png"));

	//Define the blocks
	SDL_Rect block = {900, 2900, 200, 20};
	SDL_Rect anotherBlock = {SCREEN_WIDTH/2 - 190, SCREEN_HEIGHT-120, 120, 20};
	SDL_Rect spring = {SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT-180, 100, 20};

	std::vector <SDL_Rect> blocks = {block, anotherBlock, spring};

	SDL_Event e;
	bool gameon = true;
	while (gameon)
	{

		user->applyForces();

		//get intended motion based off input
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				gameon = false;
			}
		}
		const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
		if(keystate[SDL_SCANCODE_W]){

				if (!user->cantJump)
				{
					user->y_vel += -15;
					user->cantJump = true;
				}

			}
		if(keystate[SDL_SCANCODE_A]){
				user->x_accel = -0.5;
					// if(user->x_vel > -4){
					// 	user->x_vel += -2;
					// }
			}
		if(keystate[SDL_SCANCODE_D]){
				user->x_accel = 0.5;
					// if(user->x_vel < 4){
					// 	user->x_vel += 2;
					// }
			}
		if(keystate[SDL_SCANCODE_S]){
					user->y_accel = 0.5;
			}

		if (!keystate[SDL_SCANCODE_A] && !keystate[SDL_SCANCODE_D])
			{
				user->x_accel = 0;
			}
	
		// Move box
		user->updatePosition();
		//new
		if (user->y_pos < (scroll_offset + tthird))
			scroll_offset = user->y_pos - tthird;
		else if (user->y_pos > (scroll_offset + bthird))
			scroll_offset = user->y_pos - bthird;

		if (scroll_offset < 0)
			scroll_offset = 0;

		if (scroll_offset + SCREEN_HEIGHT > WORLD_DEPTH)
			scroll_offset = WORLD_DEPTH - SCREEN_HEIGHT;
		//check constraints and resolve conflicts
		//apply forces based off gravity and collisions
		user->detectCollisions(blocks);
		
		// Clear black
		SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(screen->renderer);
		//new
		rem = scroll_offset % SCREEN_HEIGHT;
		bgRect.y = -rem;
		SDL_RenderCopy(screen->renderer, gBackground, nullptr, &bgRect);
		bgRect.y += SCREEN_HEIGHT;
		SDL_RenderCopy(screen->renderer, gBackground, nullptr, &bgRect);
		// Draw boxes
		SDL_SetRenderDrawColor(screen->renderer, 0xFF, 0x00, 0x00, 0xFF);
		

		//new
		playercam.y = user->y_pos - scroll_offset;
		playercam.x = user->x_pos;
		// Player box
		SDL_Rect player_rect = {user->x_pos, user->y_pos, user->width, user->height};
		SDL_RenderCopy(screen->renderer, user->player_texture, NULL, &playercam);
		
		for (auto b: blocks)
		{//on screen  if b.y = bgRect.y to bgRect.y + 720
	//position on screen = b.y - bgRect.y
			if(b.y >= bgRect.y && b.y <= bgRect.y + 720)
			{
				SDL_Rect tempBlock = {b.x, b.y-bgRect.y,b.w, b.h};
				SDL_RenderFillRect(screen->renderer, &tempBlock);
			}
		}
		
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
void generateTerrain()
{
	SimplexNoise *simp = new SimplexNoise(420);
	simp->freq = 0.07f;
	simp->octaves = 2;
	simp->updateFractalBounds();
	int cave_nums[SCREEN_HEIGHT / TILE_SIZE];
	//load texture
	SDL_Texture* block_texture = loadTexture("../res/block.png");
	SDL_Texture* background_texture = loadTexture("../res/background_block.png");
	
	for (int test = -18; test < 18; test++)
	{
		float val_f = simp->getSingle(1,test)*100;
		int val_i = (int)val_f;
		cave_nums[test+18] = val_i;
	}
	for (int y = 0; y < SCREEN_HEIGHT; y = y + TILE_SIZE)
	{
		for(int x = 0; x < SCREEN_WIDTH; x = x + TILE_SIZE)
		{
			int ratio = (float)x / (float)SCREEN_WIDTH * 100;
			if((ratio <cave_nums[y / TILE_SIZE] - 10) || (ratio > cave_nums[y / TILE_SIZE] + 10))
			{
				SDL_Rect block = {x,y,TILE_SIZE,TILE_SIZE};
				SDL_RenderCopy(screen->renderer,block_texture,NULL,&block);
			}
			else
			{
				SDL_Rect block = {x,y,TILE_SIZE,TILE_SIZE};
				SDL_RenderCopy(screen->renderer,background_texture,NULL,&block);
			}
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