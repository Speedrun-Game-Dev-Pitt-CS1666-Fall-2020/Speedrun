// SDL2 stuffs
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

// Networking stuffs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

// Our headers
#include "XorShifter.h"
#include "SimplexNoise.h"
#include "Screen.h"
#include "Image.h"
#include "Player.h"
#include "Block.h"
#include "BouncyBlock.h"

#include "MenuStateMachine.hpp"

#include "Block.h"

#define CREDIT_SIZE 10
#define MENU_SIZE 5

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

//for move player tutorial, may move to player object later
constexpr int BOX_SIZE = 20;
constexpr int WORLD_DEPTH = 7200;

constexpr int WORLD_HEIGHT = 720 * 10 + 20 * BOX_SIZE * 3; // 3 caverns, each 20 boxes tall

// Globals
Screen *screen = nullptr;
Uint32 before;
Uint32 then;
Uint32 delta;
Uint32 now;
std::vector <Block> blocks;	//stores collidable blocks


std::vector <BouncyBlock> bouncyblocks;

std::vector <SDL_Rect> decorative_blocks;	//stores non-collidable blocks
int clientSocket;	//Socket for connecting to the server

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

void drawOtherPlayers(Player* thisPlayer, float otherPlayerOriginX, float otherPlayerOriginY, int playerNum)
{
	float otherPlayerScreenY = thisPlayer->y_screenPos - (thisPlayer->y_pos - otherPlayerOriginY);

	std::string spriteName = "../res/Guy" + std::to_string(playerNum) + std::string(".png");
	SDL_Rect player = {otherPlayerOriginX, otherPlayerScreenY, thisPlayer->width, thisPlayer->height};
	std::cout << "Placing player " << playerNum << " at " << otherPlayerOriginX << ", " << otherPlayerScreenY << std::endl;
	SDL_RenderCopy(screen->renderer, loadTexture((spriteName)), NULL, &player);
	SDL_RenderPresent(screen->renderer);
}

#include "GameMap.h"
#include "MapGenerator.h"

#define MIN_DENSITY 100

Player* generateTerrain(int seed)
{
	blocks.clear();
	decorative_blocks.clear();

	GameMap* map = new GameMap(seed);
	MapGenerator* mg = new MapGenerator(map);

	//compress voxel into block array
	for(int y = 0; y < map->h; y++){
		/*
		for(int x = 0; x < map->w; x++){
			if(map->field[x][y] >= 100){
				SDL_Rect rect = {x*BOX_SIZE, y*BOX_SIZE, BOX_SIZE, BOX_SIZE};
				Block block = Block(rect, 0); //normal block
				blocks.push_back(block);
			}
		}
		*/
		
		int sx = 0;
		int w = 0;
		int prevtype = -1; //-1 implies air
		int x = 0;
		for(; x < map->w; x++){
			int currtype = map->field[x][y] >= 100 ? map->mat_field[x][y] : -1;
			
			if(currtype > -1){
				if(prevtype==currtype){
					w++;
				}else{
					if(prevtype > -1){
						SDL_Rect rect = {sx*BOX_SIZE, y*BOX_SIZE, w*BOX_SIZE, BOX_SIZE};
						Block block = Block(rect, prevtype); //normal block
						blocks.push_back(block);
					}
					sx = x;
					w = 1;
				}
			}else{
				if(prevtype > -1){
					SDL_Rect rect = {sx*BOX_SIZE, y*BOX_SIZE, w*BOX_SIZE, BOX_SIZE};
					Block block = Block(rect, prevtype); //normal block
					blocks.push_back(block);
				}
				w = 0;
			}

			prevtype = currtype;
			/*
			if(type==currtype){//grow rect
				w++;
			}else if(currtype){//start rect
				sx = x;
				w = 0;
			}else{//end rect
				SDL_Rect rect = {sx*BOX_SIZE, y*BOX_SIZE, w*BOX_SIZE, BOX_SIZE};
				Block block = Block(rect, 0); //normal block
				blocks.push_back(block);
				w = 0;
				sx = x;

			}
			type = currtype;
			*/
		}

		if(prevtype>-1){
			SDL_Rect rect = {sx*BOX_SIZE, y*BOX_SIZE, w*BOX_SIZE, BOX_SIZE};
			Block block = Block(rect, prevtype); //normal block
			blocks.push_back(block);
		}
	}
	std::cout << "size: " << blocks.size() << "\n";
	std::cout << "size: " << blocks.size() << "\n";
	for(Block block : blocks){
		//std::cout << block.block_rect.x << ", " <<  block.block_rect.y << ", " << block.block_rect.w << ", " << block.block_rect.h << "\n";
	}


	//spawn player (we need to put this in its own function)
	return new Player(mg->tubePoints[0].x*BOX_SIZE, mg->tubePoints[0].y*BOX_SIZE, 20, 20, loadTexture("../res/Guy.png"));
}

void renderTerrain(Player* p){
	int tx = p->x_pos-(1280/2);
	int ty = p->y_pos-(720/2);
	for(Block b : blocks){
		SDL_Rect rect = {b.block_rect.x-tx,b.block_rect.y-ty,b.block_rect.w,b.block_rect.h};
		SDL_SetRenderDrawColor(screen->renderer, b.red, b.green, b.blue, 0xFF);
		SDL_RenderFillRect(screen->renderer, &rect);
	}
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

	Player *user = NULL;
	char buffer[256];
	bzero(buffer, 256);

	if(multiplayer)
	{
		srand(time(NULL));
		int rand (void);

		bzero(buffer, 256);
		int lR = sprintf(buffer, "%i", (rand() + 2));
		int n = write(clientSocket, buffer, strlen(buffer));

		if (n < 0)
		{
			herror("Error writing random number to server...");
		}

		bzero(buffer, 256);
		n = read(clientSocket, buffer, 255);

		if (n < 0)
		{
			herror("ERROR reading from socket");
		}
		else
		{
			int seed = 1;
			seed = atoi(buffer);
			user = generateTerrain(seed);
		}
	}
	else
	{
		//create the player and generate the terrain
		user = generateTerrain(time(NULL));
	}
	SDL_Rect bee = {user->x_pos, user->y_pos+20, BOX_WIDTH*5, BOX_HEIGHT};
	Block* hellothere = new Block(bee, 1, true, 1, 80); //normal block
	blocks.push_back(*hellothere);


	BouncyBlock* bouncy = new BouncyBlock(user->x_pos, user->y_pos+20, BOX_WIDTH, BOX_HEIGHT, 2, 2);
	BouncyBlock* bouncy2 = new BouncyBlock(user->x_pos+20, user->y_pos+20, BOX_WIDTH, BOX_HEIGHT, 3, -1);

	bouncyblocks.push_back(*bouncy);
	bouncyblocks.push_back(*bouncy2);

	//Define the blocks
	/*SDL_Rect block = {SCREEN_WIDTH/2, SCREEN_HEIGHT-20, 200, 20};
	SDL_Rect anotherBlock = {SCREEN_WIDTH/2 - 190, SCREEN_HEIGHT-120, 120, 20};
	SDL_Rect spring = {SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT-180, 100, 20};
	blocks = {block, anotherBlock, spring};*/

	then = 0;
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

		//apply forces on all bouncys
		for (int i=0; i<bouncyblocks.size(); i++)
		{
			BouncyBlock temp = bouncyblocks.at(i);
			temp.applyForces();
			bouncyblocks.at(i) = temp;
		}

		//get intended motion based off input
		SDL_PollEvent(&e);

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
		user->updatePosition();

		//move bouncies
		for (int i=0; i<bouncyblocks.size(); i++)
		{
			BouncyBlock temp = bouncyblocks.at(i);
			temp.updatePosition();
			bouncyblocks.at(i) = temp;
		}
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

			std::cout << "Hello! My positions is " << user->x_pos << ", " << user->y_pos << std::endl;

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

		//for all moving blocks, update position and time counter
		for (int i=0; i<blocks.size(); i++)
		{
			if(blocks.at(i).moving){
				Block temp = blocks.at(i);
				temp.updatePosition();
				blocks.at(i) = temp;
			}
		}
		// Draw boxes
		//SDL_SetRenderDrawColor(screen->renderer, 0xFF, 0x00, 0x00, 0xFF);

    renderTerrain(user);
		//detect collisions for all bouncy blocks
		for (int i=0; i<bouncyblocks.size(); i++)
		{
			BouncyBlock temp = bouncyblocks.at(i);
			temp.detectCollisionsBlock(blocks);
			bouncyblocks.at(i) = temp;
		}

		user->detectCollisions(blocks);
		user->detectBouncyBlockCollisions(bouncyblocks);

		float mX = 0;
		float mY = 0;

		if(multiplayer == true){

			bzero(buffer, 256);
			int n = read(clientSocket, buffer, 255);

			if (n < 0) {
				herror("ERROR reading from socket");
			}
			else {
				int playerCount;
				int currPlayer = 2;
				int hitMarks = 0;
				int currentMark = -1;
				int incX = 0;
				int incY = 0;
				int index = 0;
				char buffX[16];
				char buffY[16];

				while(hitMarks < 3)
				{
					if(hitMarks == 2)
					{
						char playerCountTemp[2];
						playerCountTemp[1] = buffer[index];
						playerCount = atoi(playerCountTemp) + 1;
					}

					if(buffer[index] == '|')
					{
						hitMarks++;
					}

					index++;
				}

				hitMarks = 0;

				for(index; index < (unsigned)strlen(buffer); index++)
				{

					if(buffer[index] == '|')
					{
						currentMark = index;
						hitMarks++;

						if(hitMarks % 2 == 0)
						{
							incX = 0;
							incY = 0;

							mX = strtof(buffX, NULL);
							mY = strtof(buffY, NULL);
							drawOtherPlayers(user, mX, mY, currPlayer);

							currPlayer++;

							/* Taking this out draws player 3 but keeps him attached to player 2's X and shifted below in Y
							if(currPlayer > playerCount) // 3
							{
								break;
							}
							*/
						}
					}
					else if(buffer[index] == 'n')
					{
						std::cout << "player " << currPlayer << "is disconnected" << std::endl;
					}
					else if(currentMark == -1)
					{
						buffX[incX] = buffer[index];
						incX++;
					}
					else if(currentMark != -1)
					{
						buffY[incY] = buffer[index];
						incY++;
					}

				}

			}
		}

		// Player box

		if(multiplayer == true)
		{
			std::cout << "Client at position: (" << mX << ", " << mY << ")" << std::endl;
		}

		//render bouncy blocks
		for (int i=0; i<bouncyblocks.size(); i++)
		{
			BouncyBlock temp = bouncyblocks.at(i);
			
			SDL_SetRenderDrawColor(screen->renderer, 0xAA, 0xBB, 0xCC, 0xFF);
			SDL_Rect bounce_rect = {temp.x_pos, (user->y_screenPos + temp.y_pos) - user->y_pos, temp.width, temp.height};
			SDL_RenderFillRect(screen->renderer, &(bounce_rect));

			bouncyblocks.at(i) = temp;
		}

		SDL_Rect player_rect = {1280/2, 720/2, user->width, user->height};

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
	Image* seedBG = loadImage("../res/SeedNotice.png", 1280, 720); 

	Image* single = loadImage("../res/MenuSingle.png", 450, 80);
	Image* credits = loadImage("../res/MenuCredits.png", 450, 80);
	Image* multi = loadImage("../res/MenuMulti.png", 920, 80);

	Image* seed = loadImage("../res/EnterSeed.png", 450, 80);
	Image* join = loadImage("../res/JoinGame.png", 450, 80);

	Image* singleSel = loadImage("../res/MenuSingleSelect.png", 450, 80);
	Image* creditsSel = loadImage("../res/MenuCreditsSelect.png", 450, 80);
	Image* multiSel = loadImage("../res/MenuMultiSelect.png", 920, 80);

	Image* seedSel = loadImage("../res/EnterSeedSelect.png", 450, 80);
	Image* joinSel = loadImage("../res/JoinGameSelect.png", 450, 80);

	// A variable representing which menu screen we're on (changes when you hit enter)
	int menuScreen = 0;

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
				single, credits, multi, seed, join
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
					case MultiR:
						menuScreen = 1;
						m.processInput(Up, menuScreen); // force the default menu 1 screen (on Seed)
						break;
					case JoinGame:
						setupMultiplayer();
						close(clientSocket);
						break;
					default:
						break;
				}
			}
			else if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) { buttonPress = Up; buttonPressed = true; }
			else if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) { buttonPress = Left; buttonPressed = true; }
			else if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) { buttonPress = Down; buttonPressed = true; }
			else if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) { buttonPress = Right; buttonPressed = true; }
			// else if (keystate 1 or numpad 1) { append 1 to seed box }

			// default init the variable to the current state in case a button wasn't pressed
			MenuState currentState = m.getState();
			if (buttonPressed) {
				// update it in accordance with the pressed button
				currentState = m.processInput(buttonPress, menuScreen);
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
				case Seed:
					menuState[3] = seedSel;
					break;
				case JoinGame:
					menuState[4] = joinSel;
					break;
			}

			// x position, y position, width, height
			SDL_Rect SpeedrunLogo = {319, 96, 642, 215};
			SDL_Rect SingleButton = {180, 390, 450, 80};
			SDL_Rect CreditsButton = {650, 390, 450, 80};
			SDL_Rect MultiButton = {180, 485, 920, 80};

			SDL_Rect SeedNotice = {363, 600, 554, 77};
			SDL_Rect SeedLabel = {415, 390, 450, 80};
			SDL_Rect JoinButton = {415, 485, 450, 80};

			// Render the background first so it's in the back!
			SDL_RenderCopy(screen->renderer, menuBG->texture, NULL, screen->bounds);
			SDL_RenderCopy(screen->renderer, logo->texture, NULL, &SpeedrunLogo);

			switch (menuScreen) {
				case 0:
					SDL_RenderCopy(screen->renderer, menuState[0]->texture, NULL, &SingleButton);
					SDL_RenderCopy(screen->renderer, menuState[1]->texture, NULL, &CreditsButton);
					SDL_RenderCopy(screen->renderer, menuState[2]->texture, NULL, &MultiButton);
					break;
				case 1:
					SDL_RenderCopy(screen->renderer, seedBG->texture, NULL, &SeedNotice);
					SDL_RenderCopy(screen->renderer, menuState[3]->texture, NULL, &SeedLabel);
					SDL_RenderCopy(screen->renderer, menuState[4]->texture, NULL, &JoinButton);
					break;
				default:
					std::cout << "Something went horribly wrong." << std::endl << menuScreen << " is not part of the domain of MenuStateMachine." << std::endl;
					exit(1);
					break;
			}

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
