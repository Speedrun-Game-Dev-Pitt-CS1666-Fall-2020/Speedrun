#pragma once
#include <SDL.h>

class Screen {
public:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Rect* bounds;

	Screen(const char* title, int w, int h);

	~Screen();

	inline bool isClosed() const { return _closed; }

	void pollEvents();

private:
	bool init(const char* title, int w, int h);
	bool _closed;

	
};