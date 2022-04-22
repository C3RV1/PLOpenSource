#ifndef K4SDL_FONTLOADER
#define K4SDL_FONTLOADER

namespace k4sdl {
    class FontLoader;
}

#include <string>
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Font.hpp"
#include "../sprite/Text.hpp"
#include <iostream>
#include <fstream>

namespace k4sdl {
    class FontLoader {
    public:
        virtual void load(std::string path, int size, Text &text) = 0;
    };

    class FontLoaderOS : public FontLoader {
    public:
        FontLoaderOS(std::string l_basePath = "", std::string l_fallBack = "") : basePath(l_basePath), fallBack(l_fallBack) {};
        virtual void load(std::string path, int size, Text &text);
    private:
        std::string basePath;
        std::string fallBack;
    };
}

#endif
