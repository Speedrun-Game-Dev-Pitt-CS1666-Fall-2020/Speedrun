#include "Renderer.h"
#include "Sprite.h"
#include "Game.h"

void Renderer::draw_fullscreen(const Sprite* sprite){
    SDL_RenderCopy(Game::screen->renderer, sprite->texture, sprite->bounds, Game::screen->bounds);
}