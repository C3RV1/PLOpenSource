#ifndef K4SDL_SPRITELOADER
#define K4SDL_SPRITELOADER

namespace k4sdl {
    class SpriteLoader;
}

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Sprite.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include "../Screen.hpp"

namespace k4sdl {
    class SpriteLoader {
    public:
        virtual void load(std::string path, Sprite &sprite,
                          bool spriteSheet = false, SDL_Color* colorKey = NULL) = 0;
    };

    class SpriteLoaderOS : public SpriteLoader {
    public:
        SpriteLoaderOS(std::string l_basePath = "") : basePath(l_basePath) {};
        virtual void load(std::string path, Sprite &sprite,
                          bool spriteSheet = false, SDL_Color* colorKey = NULL);
    private:
        std::string basePath;
    };
}

#endif
