// SDL2 stuffs
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

// Networking stuffs
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Our headers
#include "XorShifter.h"
#include "SimplexNoise.h"
#include "Screen.h"
#include "Image.h"
#include "Player.h"
#include "MenuStateMachine.hpp"

#define CREDIT_SIZE 10
#define MENU_SIZE 3

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

//for move player tutorial, may move to player object later
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;
constexpr int WORLD_DEPTH = 7200;

constexpr int WORLD_HEIGHT = 720 * 10 + 20 * BOX_HEIGHT * 3; // 3 caverns, each 20 boxes tall

// Globals
Screen *screen = nullptr;
Uint32 before;
Uint32 then;
Uint32 delta;
Uint32 now;
std::vector <SDL_Rect> blocks;	//stores collidable blocks
std::vector <SDL_Rect> decorative_blocks;	//stores non-collidable blocks
int clientSocket;	//Socket for connecting to the server

// Function declarations
bool init();
void close();
Image *loadImage(const char *, int, int);
void runCredits();
void runGame();
void runMenu();
Player* generateTerrain();
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

//create blocks, add them to the blocks vector, and return a player at a valid spawn point
Player* generateTerrain()
{
	blocks.clear();
	decorative_blocks.clear();

	srand (time(NULL));
	int rand (void);

	XorShifter *rng = new XorShifter(412001000);

	SimplexNoise *simp = new SimplexNoise(rand() % 1000000);
	simp->freq = 0.05f;
	simp->octaves = 2;
	simp->updateFractalBounds();

	int cave_nums[WORLD_HEIGHT / BOX_HEIGHT];


	//generate values from a "line" of noise, one value for each row of blocks
	int range = WORLD_HEIGHT/BOX_WIDTH/2;
	for (int test = -range; test < range; test++)
	{
		float val_f = simp->getSingle(1, test) * 100;

		int val_i = (int)val_f;

		cave_nums[test + range] = val_i;
	}

	//cave_area is a boolean array that tracks which blocks are part of the walls and which are part of the cave
	//a value of "true" means that the corresponding block is part of the cave, and will not be rendered
	bool cave_area[WORLD_HEIGHT/BOX_WIDTH][SCREEN_WIDTH/BOX_WIDTH];
	for(int i = 0; i < WORLD_HEIGHT/BOX_WIDTH; i++)
	{
		for(int j = 0; j < SCREEN_WIDTH/BOX_WIDTH; j++)
		{
			cave_area[i][j]=false;
		}
	}

	bool player_created = false;
	Player *user;
	int cave_nums_index = 0;
	
	//"gaps" define where the caverns will be placed
	int gap1 = (rand() % 20) + 20;
	int gap2 = (rand() % 20) + 70;
	int gap3 = (rand() % 20) + 120;

	//for each block on the screen
	for (int y = 0; y < WORLD_HEIGHT; y = y + BOX_HEIGHT)
	{
		//place cavern at current location
		if (y / BOX_HEIGHT == gap1 || y / BOX_HEIGHT == gap2 || y / BOX_HEIGHT == gap3)		
		{
			//cavern should be 20 boxes tall
			int cavern_end = y + 20 * BOX_HEIGHT;
			int count = 0;
			for (; y < cavern_end; y = y + BOX_HEIGHT)
			{
				for (int x = 0; x < SCREEN_WIDTH; x = x + BOX_WIDTH)
				{
					int x_left = (x - BOX_WIDTH)/BOX_WIDTH;
					if (x_left < 0)
					{
						x_left = 0;
					}
					
					int x_right = (x + BOX_WIDTH)/BOX_WIDTH;
					if (x_right >= SCREEN_WIDTH)
					{
						x_right = (SCREEN_WIDTH - BOX_WIDTH)/BOX_WIDTH;
					}

					//expand cavern size
					if (count < 5 && (cave_area[(y - BOX_WIDTH)/BOX_WIDTH][x/BOX_WIDTH] == true ||
					    cave_area[(y - BOX_WIDTH)/BOX_WIDTH][x_left] == true ||
					    cave_area[(y - BOX_WIDTH)/BOX_WIDTH][x_right] == true))
					{
						cave_area[y/BOX_WIDTH][x/BOX_WIDTH] = true;
					}
					//keep cavern size constant
					else if (count >= 5 && count < 15 && cave_area[(y - BOX_WIDTH)/BOX_WIDTH][x/BOX_WIDTH] == true)
					{
						cave_area[y/BOX_WIDTH][x/BOX_WIDTH] = true;
					}
					//shrink cavern size
					else if (count >= 15 && count < 20 && (cave_area[(y - BOX_WIDTH)/BOX_WIDTH][x/BOX_WIDTH] == true &&
					         cave_area[(y - BOX_WIDTH)/BOX_WIDTH][x_left] == true &&
					         cave_area[(y - BOX_WIDTH)/BOX_WIDTH][x_right] == true))
					{
						cave_area[y/BOX_WIDTH][x/BOX_WIDTH] = true;
					}
				}
				count++;
			}
			continue;
		}
		bool b = true;
		
		//"start" indicates the relative position of the left wall of the cave to the screen at a given elevation
		int start = cave_nums[cave_nums_index] - 11;

		//"end" indicates the relative position of the right wall of the cave to the screen at a given elevation
		int end = cave_nums[cave_nums_index] + 10;

		//for each block at elevation y, compare the relative x position of the block on the screen to the
		//"start" and "end" positions
		for (int x = 0; x < SCREEN_WIDTH; x = x + BOX_WIDTH)
		{
			//relative x position of the block on the screen
			int ratio = (float)x / (float)SCREEN_WIDTH * 100;

			//if ratio is between start & end, we are in the cave
			if (ratio > start && ratio < end)
			{
				//indicate that we are in the cave by marking the corresponding location in array as "true"
				cave_area[y/BOX_WIDTH][x/BOX_WIDTH] = true;


				//mark the blocks above and below the current block as being in the cave, to increase width
				if (y != 0)
				{
					int y_above = (y - BOX_WIDTH)/BOX_WIDTH;
					if (y_above >= 0)
					{
						cave_area[y_above][x/BOX_WIDTH] = true;
					}
				}
				if (y != WORLD_HEIGHT - BOX_HEIGHT)
				{
					int y_below = (y + BOX_WIDTH)/BOX_WIDTH;
					if (y_below <= WORLD_HEIGHT/BOX_WIDTH)
					{
						cave_area[y_below][x/BOX_WIDTH] = true;
					}
				}
			}
		}
		
		cave_nums_index++;
	}

	//use our cave_area array to determine where to render blocks
	for (int y = 0; y < WORLD_HEIGHT; y = y + BOX_HEIGHT)
	{

		bool b = true;
		for (int x = 0; x < SCREEN_WIDTH; x = x + BOX_WIDTH)
		{
			//create left wall of cave
			if (cave_area[y/BOX_WIDTH][x/BOX_WIDTH] && b)
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

			//create right wall of cave
			if (!cave_area[y/BOX_WIDTH][x/BOX_WIDTH] && !b)
			{
				SDL_Rect block = {x, y, BOX_WIDTH *100, BOX_HEIGHT};
				blocks.push_back(block);
				break;
			}
			else if (x == SCREEN_WIDTH - BOX_WIDTH)
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
			int index = (now - before) / 7200;
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

void runGame(bool multiplayer)
{
	// Create player object with x, y, w, h, texture
	//Player *user = new Player(10, 0, 20, 20, loadTexture("../res/Guy.png"));

	//create the player and generate the terrain
	Player *user = generateTerrain();


	//Define the blocks
	/*SDL_Rect block = {SCREEN_WIDTH/2, SCREEN_HEIGHT-20, 200, 20};
	SDL_Rect anotherBlock = {SCREEN_WIDTH/2 - 190, SCREEN_HEIGHT-120, 120, 20};
	SDL_Rect spring = {SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT-180, 100, 20};
	blocks = {block, anotherBlock, spring};*/
	then = 0;
	char buffer[256];
	bzero(buffer, 256);
	SDL_Event e;
	bool gameon = true;
	while (gameon)
	{
    now = SDL_GetTicks();

		if (now - then < 16)
			continue;

		then = now;	
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
					user->x_accel = -1;
					// if(user->x_vel > -4){
					// 	user->x_vel += -2;
					// }
				}
				if(keystate[SDL_SCANCODE_D]){
					user->x_accel = 1;
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
		//if user position on screen < 720/3
			//then change user position and user position on screen, not blocks.
		//if user position on screen is > 1440/3
			//then change user position and user position on screeen, not blocks.
		//else
			//change block locations
		//X, always change X of player and player screen pos, never block
		
		//check constraints and resolve conflicts
		//apply forces based off gravity and collisions
		
		if(multiplayer == true)
		{
			char xBuff[16];
			int lX = sprintf(xBuff, "%.5f", user->x_pos);
			char yBuff[16];
			int lY = sprintf(yBuff, "%.5f", user->y_pos);

			bzero(buffer, 256);

			int incX = 0;
			int incY = 0;

			for(int i = 0; i < lX + lY + 1; i++)
			{

				if(i < lX)
				{
					buffer[i] = xBuff[incX];
					incX++;
				}
				else if(i == lX)
				{
					buffer[i] = '|';
				}
				else
				{
					buffer[i] = yBuff[incY];
					incY++;
				}

			}

			int n = write(clientSocket, buffer, strlen(buffer));


			if (n < 0)
			{
				herror("Error writing to server...");
			}
		}

		// Clear black
		SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(screen->renderer);

		// Draw boxes
		SDL_SetRenderDrawColor(screen->renderer, 0xFF, 0x00, 0x00, 0xFF);
		
		if(user->y_screenPos < 720/3 )
		{
			user->y_screenPos += user->y_vel;//problem on start
			for (auto b: blocks)
			{
				SDL_RenderFillRect(screen->renderer, &b);
			}
		}
		else
		{
			for (auto b: blocks)
			{
				b.y -= (user->y_pos-user->y_screenPos);
				SDL_RenderFillRect(screen->renderer, &b);
			}
		}
		user->detectCollisions(blocks);
		
		float mX = 0;
		float mY = 0;

		if(multiplayer == true)
		{
			bzero(buffer, 256);
			int n = read(clientSocket, buffer, 255);

			if (n < 0) {
				herror("ERROR reading from socket");
			}
			else
			{
				int currentMark = -1;
				int incX = 0;
				int incY = 0;
				char buffX[16];
				char buffY[16];

				for(int i = 0; i < 17; i++)
				{

					if(buffer[i] == '|')
					{
						currentMark = i;
					}
					else if(currentMark == -1)
					{
						buffX[incX] = buffer[i];
						incX++;
					}
					else if(currentMark != -1)
					{
						buffY[incY] = buffer[i];
						incY++;
					}

				}

				mX = strtof(buffX, NULL);
				mY = strtof(buffY, NULL);
			}
		}
		// Player box

		if(multiplayer == true)
		{
			std::cout << "Client at position: (" << mX << ", " << mY << ")" << std::endl;
		}
		
		SDL_Rect player_rect = {user->x_pos, user->y_screenPos, user->width, user->height};
		SDL_RenderCopy(screen->renderer, user->player_texture, NULL, &player_rect);
		SDL_RenderPresent(screen->renderer);
	}
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void setupMultiplayer()
{

	const char* hostName = "localhost";
	const uint16_t portNum = 3060;
	char buffer[256];

	//Create socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0) {
		error("Unable to create socket for networking.");
	}

	//Create server object
	struct sockaddr_in serverAddress;
	bzero((char*) &serverAddress, sizeof(serverAddress));

	//Get server info
	struct hostent* server = gethostbyname(hostName);
	if(server == NULL) {
		error("Unable to find server.");
	}

	//Populate server object
	serverAddress.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serverAddress.sin_addr.s_addr, server->h_length);

	//Connect to server
	serverAddress.sin_port = htons(portNum);
	if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		error("Error connecting to server");
	}

	runGame(true);

}

void runMultiTestClient()
{
	// Sockets Linux tutorial:
	// http://www.linuxhowtos.org/C_C++/socket.htm

	/*
	1. Create a socket with the socket() system call
	2. Connect the socket to the address of the server using the connect() system call
	3. Send and receive data. There are a number of ways to do this, but the simplest is to use the read() and write() system calls.
	*/

	/*
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


	// Start the game!
	runGame();

	// Player disconnected
	close(clientSocket);
	*/
}

void runMenu()
{
	SDL_Event e;
	bool gameon = true;
	bool menuon = true;
	int menuPos = 0;

	// The menu is a FSM!
	MenuStateMachine m;

	// Load in our menu images
	Image* logo = loadImage("../res/SpeedrunLogo.png", 642, 215);
	Image* menuBG = loadImage("../res/FadedBackground.png", 1280, 720);

	Image* single = loadImage("../res/MenuSingle.png", 450, 80);
	Image* credits = loadImage("../res/MenuCredits.png", 450, 80);
	Image* multi = loadImage("../res/MenuMulti.png", 920, 80);

	Image* singleSel = loadImage("../res/MenuSingleSelect.png", 450, 80);
	Image* creditsSel = loadImage("../res/MenuCreditsSelect.png", 450, 80);
	Image* multiSel = loadImage("../res/MenuMultiSelect.png", 920, 80);

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
			bool buttonPressed = false;
			MenuInput buttonPress;

			// initialize all buttons to unselected
			Image* menuState[MENU_SIZE] = {
				single, credits, multi
			};

			if (keystate[SDL_SCANCODE_RETURN]) {
				switch (m.getState()) {
					case Single:
						runGame(false);
						break;
					case Credits:
						before = SDL_GetTicks();
						runCredits();
						break;
					case MultiL:
						setupMultiplayer();
						close(clientSocket);
						break;
					case MultiR:
						setupMultiplayer();
						close(clientSocket);
						break;
				}
			}
			else if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) { buttonPress = Up; buttonPressed = true; }
			else if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) { buttonPress = Left; buttonPressed = true; }
			else if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) { buttonPress = Down; buttonPressed = true; }
			else if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) { buttonPress = Right; buttonPressed = true; }

			// default init the variable to the current state in case a button wasn't pressed
			MenuState currentState = m.getState();
			if (buttonPressed) {
				// update it in accordance with the pressed button
				currentState = m.processInput(buttonPress);
			}

			// switch out unselected for selected on the correct button
			switch (currentState) {
				case Single:
					menuState[0] = singleSel;
					break;
				case Credits:
					menuState[1] = creditsSel;
					break;
				case MultiL:
				case MultiR:
					menuState[2] = multiSel;
					break;
			}

			// x position, y position, width, height
			SDL_Rect SpeedrunLogo = {319, 96, 642, 215};
			SDL_Rect SingleButton = {180, 390, 450, 80};
			SDL_Rect CreditsButton = {650, 390, 450, 80};
			SDL_Rect MultiButton = {180, 485, 920, 80};

			// Render the background first so it's in the back!
			SDL_RenderCopy(screen->renderer, menuBG->texture, NULL, screen->bounds);
			SDL_RenderCopy(screen->renderer, logo->texture, NULL, &SpeedrunLogo);
			SDL_RenderCopy(screen->renderer, menuState[0]->texture, NULL, &SingleButton);
			SDL_RenderCopy(screen->renderer, menuState[1]->texture, NULL, &CreditsButton);
			SDL_RenderCopy(screen->renderer, menuState[2]->texture, NULL, &MultiButton);

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
