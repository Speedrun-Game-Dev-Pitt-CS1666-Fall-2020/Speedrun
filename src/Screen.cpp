#include "Screen.h"
#include <iostream>
#include "Player.h"

Screen::Screen(const char* title, int w, int h) {
	_closed = !init(title, w, h);
}

Screen::~Screen() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	delete bounds;
	SDL_Quit();
}

bool Screen::init(const char* title, int w, int h) {
	this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::cerr << "Failed to create window." << std::endl;
		return false;
	}
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		std::cerr << "Failed to create renderer." << std::endl;
		return false;
	}
	this->bounds = new SDL_Rect{0, 0, w, h};
	return true;
}
void Screen::updatePosition(Player user){
		bounds->x = user.x_pos - 640;
		bounds->y = user.y_pos -360;
		if(bounds->x > bounds->w)
			bounds->x = bounds->w;
		if(bounds->x < 0)
			bounds->x = 0;
		if(bounds->y > bounds->h)
			bounds->y = bounds->h;
		if(bounds->y < 0)
			bounds->y = 0;
}
void Screen::pollEvents() {
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			_closed = true;
			break;
		default:
			break;
		}
	}
}