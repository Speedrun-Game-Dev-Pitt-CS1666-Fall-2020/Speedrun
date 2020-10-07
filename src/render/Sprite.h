#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Screen.h"


struct Sprite {
	SDL_Texture* texture;
	SDL_Rect* bounds;
};