#include "SpriteLib.h"

#include "Game.h"
#include "Sprite.h"

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

namespace SpriteLib {
    
    namespace{
        typedef std::string string;
        typedef const Sprite Sp;

        const string RES_PATH = "../res/";
        bool _success = true;
        Sp* get(string file_name){
            string path = RES_PATH + file_name;
            SDL_Surface* surface = IMG_Load(path.c_str());
            if (!surface) {
                std::cerr << "Failed to create surface. (From: " << path << ")" << std::endl;
                _success = false;
                return nullptr;
            }
            
            SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::screen->renderer, surface);
            if (!texture) {
                std::cerr << "Failed to create texture. (From: " << path << ")" << std::endl;
                _success = false;
                return nullptr;
            }

            Sprite* sprite = new Sprite();
            sprite->texture = texture;
            sprite->bounds = new SDL_Rect{0,0,surface->w,surface->h};

            SDL_FreeSurface(surface);
            return sprite;
        }
    }
    
    Sp** CREDITS;
}

bool SpriteLib::load(){
    CREDITS = new Sp*[10]{
        get("rjd68.png"),
        get("alex.png"),
        get("andrew.png"),
        get("cas380.png"),
        get("connor.png"),
        get("jacob.png"),
        get("lucas.png"),
        get("robert.png"),
        get("spencer.png"),
        get("ryanyang.png")
    };


    return _success;
}