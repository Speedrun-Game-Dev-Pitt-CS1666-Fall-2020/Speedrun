#include "Button.h"
#include "Game.h"
#include "Vec2.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

Button::Button(const char* label, int font_size, int x, int y, int w, int h){
    this->label = label;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->border_color = SDL_Color{0xFF,0xFF,0xFF,0xFF};
    this->bg_color = SDL_Color{0x00,0x00,0x00,0x00};//debug transparency. fully transparent red
    this->active_bg_color = SDL_Color{0xFF,0xFF,0xFF,0x55};
    this->font_color = SDL_Color{0xFF,0xFF,0xFF,0xFF};

    this->font = "../res/myfont.ttf";
    this->font_size = font_size;
    this->refresh();
}
Button::~Button(){
    if(lbl_tex!=nullptr)SDL_DestroyTexture(lbl_tex);
}

void Button::refresh(){
    TTF_Font* ttf_font = TTF_OpenFont(this->font, this->font_size);
    if(ttf_font==nullptr){
        std::cerr << "Failed to load the button font \"" << this->font << "\"." << std::endl;
        this->lbl_tex = nullptr;
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(ttf_font, this->label, this->font_color);
    if(ttf_font==nullptr){
        std::cerr << "Failed to create a button surface from ttf." << std::endl;
        this->lbl_tex = nullptr;
        return;
    }
    this->lbl_w = surface->w;
    this->lbl_h = surface->h;

    this->lbl_tex = SDL_CreateTextureFromSurface(Game::screen->renderer, surface);
    if(ttf_font==nullptr){
        std::cerr << "Failed to create a button label's texture from surface." << std::endl;
        this->lbl_tex = nullptr;
        return;
    }

    SDL_FreeSurface(surface);
}

bool Button::tick(){
    SDL_Rect rect = {x,y,w,h};
    bool _active = isHover();
    if(_active){
        SDL_SetRenderDrawColor(Game::screen->renderer, 
            this->active_bg_color.r, 
            this->active_bg_color.g,
            this->active_bg_color.b,
            this->active_bg_color.a);
        SDL_RenderFillRect(Game::screen->renderer, &rect);
    }else{
        SDL_SetRenderDrawColor(Game::screen->renderer, 
            this->bg_color.r, 
            this->bg_color.g,
            this->bg_color.b,
            this->bg_color.a);
        SDL_RenderFillRect(Game::screen->renderer, &rect);
    }

    SDL_SetRenderDrawColor(Game::screen->renderer, 
            this->border_color.r, 
            this->border_color.g,
            this->border_color.b,
            this->border_color.a);
    SDL_RenderDrawRect(Game::screen->renderer, &rect);

    if(this->lbl_tex!=nullptr){
        int lx = x+(w-lbl_w)/2;
        int ly = y+(h-lbl_h)/2;
        SDL_Rect dst = {lx,ly,lbl_w,lbl_h};
        SDL_RenderCopy(Game::screen->renderer, lbl_tex, NULL, &dst);
    }
    
    return _active && Game::cursor->isLeftClick;
}

bool Button::isHover(){
    Vec2 c = Game::cursor->screen_pos;
    return (c.x > x && c.x < x+w && c.y > y && c.y < y+h );
}