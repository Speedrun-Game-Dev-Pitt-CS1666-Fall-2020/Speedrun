#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "MenuState.h"
#include "SpriteLib.h"

bool initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Failed to load SDL" << std::endl;
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		std::cerr << "Failed to initialize SDL_image" << std::endl;
		return false;
	}
	if (TTF_Init()==-1)
	{
		std::cerr << "Failed to initialize SDL_ttf" << std::endl;
		return false;
	}

	return true;
}

bool initGame(){
	Game::screen = new Screen("Hamsterball Cave Racing", 1280, 720);
	if(Game::screen->isClosed()){
		std::cout << "Failed to initialize screen."	<< std::endl;
		return false;
	}
	if(!SpriteLib::load()){
		std::cout << "Failed at SpriteLib load." << std::endl;
		return false;
	}
	SDL_SetRenderDrawBlendMode(Game::screen->renderer, SDL_BLENDMODE_BLEND);

    Game::state_manager = new StateManager(10);
	Game::state_manager->push(new MenuState());

	Game::cursor = new Cursor();

	return true;
}

bool clean(){
	SDL_Quit();
	delete Game::screen;
	delete Game::state_manager;

	return true;
}

int main(int argc, char **argv)
{
	std::cout << "Starting debug..." << std::endl;
	// Initialize Game
	if (initSDL() && initGame())
	{

		Game::start();
		
	}else{
		std::cout << "Failed to initialize!" << std::endl;
	}


	if(clean()){
		return 0;
	}else{
		std::cout << "Failed to clean up!" << std::endl;
		return -1;
	}

	
}



/*
#define CREDIT_SIZE 10
#define MENU_SIZE 4

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

//for move player tutorial, may move to player object later
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;
constexpr int WORLD_DEPTH = 3000;

// Globals
Screen *screen = nullptr;
Uint32 before;
Uint32 then;
Uint32 delta;
Uint32 now; 
float dt;
std::vector <SDL_Rect> blocks;	//stores collidable blocks
std::vector <SDL_Rect> decorative_blocks;	//stores non-collidable blocks

// Function declarations
bool init();
void close();
Sprite* loadImage(const char *, int, int);
void runCredits();
void runGame();
void runMenu();
Player* generateTerrain();
SDL_Texture *loadTexture(std::string);

Sprite* loadImage(const char *src, int w, int h)
{
	return new Sprite(screen, src, 0, 0, w, h);
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



void close()
{
	delete screen;
	// Quit SDL subsystems
	SDL_Quit();
}

//create blocks, add them to the blocks vector, and return a player at a valid spawn point
Player* generateTerrain()
{
	blocks.clear();
	decorative_blocks.clear();
	
	srand (time(NULL));
	//int rand (void);
	
	XorShifter *rng = new XorShifter(412001000);

	SimplexNoise *simp = new SimplexNoise(rng->next() % 1000000);
	simp->freq = 0.05f;
	simp->octaves = 2;
	simp->updateFractalBounds();
	
	int cave_nums[SCREEN_HEIGHT / BOX_HEIGHT];
	
	//SDL_Texture* block_texture = loadTexture("../res/block.png");
	//SDL_Texture* background_texture = loadTexture("../res/background_block.png");
	
	//generate values from a "line" of noise, one value for each row of blocks
	for (int test = -18; test < 18; test++)
	{
		float val_f = simp->getSingle(1, test) * 100;
		
		int val_i = (int)val_f;
		
		cave_nums[test + 18] = val_i;
	}
	
	bool player_created = false;
	Player *user;// = new Player(10, 0, 20, 20, loadTexture("../res/Guy.png"));
	
	//for each block on the screen
	for (int y = 0; y < SCREEN_HEIGHT; y = y + BOX_HEIGHT)
	{
		bool b = true;
		
		//"start" indicates the relative position of the left wall of the cave to the screen at a given elevation
		int start = cave_nums[y / BOX_HEIGHT] - 11;
		
		//"end" indicates the relative position of the right wall of the cave to the screen at a given elevation
		int end = cave_nums[y / BOX_HEIGHT] + 10;
		
		//for each block at elevation y, compare the relative x position of the block on the screen to the
		//"start" and "end" positions
		for (int x = 0; x < SCREEN_WIDTH; x = x + BOX_WIDTH)
		{
			//relative x position of the block on the screen
			int ratio = (float)x / (float)SCREEN_WIDTH * 100;
			
			//create the rectangle representing the left wall of the cave at elevation y when we reach the correct relative x position
			if (ratio > start && b)
			{
				SDL_Rect block = {0, y, BOX_WIDTH * (x / BOX_WIDTH), BOX_HEIGHT};
				blocks.push_back(block);
				b = false;
				
				//spawn the player in at the first available "free" block
				if (!player_created)
				{
					user = new Player(x, y, 20, 20, loadTexture("../res/Guy.png"));
					player_created = true;
				}
			}
			//create the rectangle representing the right wall of the cave at elevation y when we reacht he correct relative x position
			if (ratio > end)
			{
				SDL_Rect block = {x, y, BOX_WIDTH *100, BOX_HEIGHT};
				blocks.push_back(block);
				break;
			}
		}
	}
	
	return user;
}

void runCredits()
{
	

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
			
			CUT OUT

			SDL_RenderPresent(screen->renderer);

			screen->pollEvents();
		}
	}
}

void runGame()
{
	// Create player object with x, y, w, h, texture
	//Player *user = new Player(10, 0, 20, 20, loadTexture("../res/Guy.png"));
	
	//create the player and generate the terrain
	Player *user = generateTerrain();

	
	//Define the blocks
	/*SDL_Rect block = {SCREEN_WIDTH/2, SCREEN_HEIGHT-20, 200, 20};
	SDL_Rect anotherBlock = {SCREEN_WIDTH/2 - 190, SCREEN_HEIGHT-120, 120, 20};
	SDL_Rect spring = {SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT-180, 100, 20};
	blocks = {block, anotherBlock, spring};*

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
					
				}

				if (!keystate[SDL_SCANCODE_A] && !keystate[SDL_SCANCODE_D])
				{
					user->x_accel = 0;
				}
			}
		}

		// Move box
		user->updatePosition();

		//check constraints and resolve conflicts
		//apply forces based off gravity and collisions
		
		// Clear black
		SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(screen->renderer);

		// Draw boxes
		SDL_SetRenderDrawColor(screen->renderer, 0xFF, 0x00, 0x00, 0xFF);
		
		for (auto b: blocks)
		{
			b.y -= (user->y_pos-user->y_screenPos);
			b.x -= (user->x_pos-user->x_screenPos);
			SDL_RenderFillRect(screen->renderer, &b);
		}
		user->detectCollisions(blocks);

		// Player box
		SDL_Rect player_rect = {user->x_screenPos, user->y_screenPos, user->width, user->height};
		SDL_RenderCopy(screen->renderer, user->player_texture, NULL, &player_rect);
		SDL_RenderPresent(screen->renderer);
	}
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void runMultiTestClient()
{
	// Sockets Linux tutorial:
	// http://www.linuxhowtos.org/C_C++/socket.htm

	/*
	1. Create a socket with the socket() system call
	2. Connect the socket to the address of the server using the connect() system call
	3. Send and receive data. There are a number of ways to do this, but the simplest is to use the read() and write() system calls.
	*

	const char* hostName = "localhost";
	const uint16_t portNum = 3060;
	char buffer[256]; // bytes to communicate

	// Create our socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {
		error("Error creating socket...");
	}

	// Create our server object
	struct sockaddr_in serverAddress;
	bzero((char*) &serverAddress, sizeof(serverAddress));

	// Get server info
	struct hostent* server = gethostbyname(hostName);
	if (server == NULL) {
		error("Host doesn't exist...");
	}

	// Populate the server object
	serverAddress.sin_family = AF_INET;
	bcopy((char*) server->h_addr, (char*) &serverAddress.sin_addr.s_addr, server->h_length);

	// Connect to the server
	serverAddress.sin_port = htons(portNum);
	if (connect(clientSocket,(struct sockaddr*) &serverAddress,sizeof(serverAddress)) < 0) {
		error("Error connecting to server...");
	}

	// Client has successfully connected to server
	// Create a message to send to it
	bzero(buffer,256);
	buffer[0] = 'P';
	buffer[1] = 'i';
	buffer[2] = 'n';
	buffer[3] = 'g';
	buffer[4] = '!';
	buffer[5] = '\n';

	// Write the data to the server
	int n = write(clientSocket,buffer,strlen(buffer));
	if (n < 0) {
		error("Error writing to server...");
	}

	/* Read what the server has to say back (Nothing)
	bzero(buffer,256);
	n = read(clientSocket,buffer,255);
	if (n < 0) {
		error("ERROR reading from socket");
	}
	printf("Read: %s\n",buffer);
	*

	// Start the game!
	runGame();

	// Player disconnected
	close(clientSocket);
}

void runMenu()
{
	SDL_Event e;
	bool gameon = true;
	bool menuon = true;
	int menuPos = 0;

	Sprite* menu[MENU_SIZE] = {
		loadImage("../res/play.png", 1280, 720),
		loadImage("../res/creds.png", 1280, 720),
		loadImage("../res/mult.png", 1280, 720),
		loadImage("../res/bees.png", 1280, 720)
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
			else if (keystate[SDL_SCANCODE_RETURN] && menuPos == 1)
			{
				before = SDL_GetTicks();
				runCredits();
			}
			else if(keystate[SDL_SCANCODE_RETURN] && menuPos == 2)
			{
				runMultiTestClient();
			}
			else if(keystate[SDL_SCANCODE_RETURN] && menuPos == 3)
			{
				//put bees
			}
			else if (keystate[SDL_SCANCODE_A] && menuPos == 1)
			{
				//can go left
				menuPos = 0;
			}
			else if (keystate[SDL_SCANCODE_D] && menuPos == 0)
			{
				//can go right
				menuPos = 1;
			}
			else if(keystate[SDL_SCANCODE_W] && menuPos == 2)
			{
				menuPos = 0;
			}
			else if(keystate[SDL_SCANCODE_W] && menuPos == 1)
			{
				menuPos = 3;
			}
			else if(keystate[SDL_SCANCODE_S] && (menuPos == 0 || menuPos == 1))
			{
				menuPos = 2;
			}
			else if(keystate[SDL_SCANCODE_S] && menuPos == 3)
			{
				menuPos = 1;
			}

			SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(screen->renderer);
			Sprite* img = menu[menuPos];
			SDL_RenderCopy(screen->renderer, img->texture, img->bounds, screen->bounds);
			SDL_RenderPresent(screen->renderer);
		}
	}
}


*/