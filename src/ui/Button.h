#pragma once

#include <SDL.h>

class Button{
public:
    const char* label;
    int x, y, w, h;
    SDL_Color border_color;
    SDL_Color bg_color;
    SDL_Color active_bg_color;
    SDL_Color font_color;

    const char* font;
    int font_size;    

    Button(const char* label, int font_size, int x, int y, int w, int h);
    ~Button();
    virtual void refresh();//use this when changing label font or fontsize
    virtual bool tick();

private:
    SDL_Texture* lbl_tex;
    int lbl_w, lbl_h;

    virtual bool isHover();
};