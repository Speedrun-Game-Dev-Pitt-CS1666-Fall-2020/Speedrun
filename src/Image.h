#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Screen.h"


class Image {
	
public:
	SDL_Texture* texture;
	SDL_Rect* bounds;

	Image(Screen* screen, const char* src, int x, int y, int w, int h) {
		auto surface = IMG_Load(src);
		if (!surface) {
			std::cerr << "Failed to create surface. (From: " << src << ")\n";
		}
		
		this->texture = SDL_CreateTextureFromSurface(screen->renderer, surface);
		if (!texture) {
			std::cerr << "Failed to create texture. (From: " << src << ")" << std::endl;
		}

		this->bounds = new SDL_Rect{ x, y, w, h };
	}

};