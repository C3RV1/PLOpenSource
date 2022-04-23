#ifndef K4SDL_FONT
#define K4SDL_FONT

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
#include "../Alignment.hpp"
#include "../Screen.hpp"

namespace k4sdl {
    class Font {
    public:
        virtual SDL_Texture* render(std::string text, SDL_Color color, SDL_Color* bgColor, int lineSpacing = 0, float hAlign = Alignment::LEFT) final;
        virtual ~Font() {};
    protected:
        virtual void setColor(SDL_Color color) = 0;
        virtual void getLineSize(std::string text, int *w, int *h) = 0;
        virtual void renderLine(SDL_Surface* surface, int x, int y, std::string text) = 0;
    };

    class TTFFont : public Font {
    public:
        TTFFont(TTF_Font* l_font) : font(l_font) {
            currentColor.r = 255;
            currentColor.g = 255;
            currentColor.b = 255;
            currentColor.a = 255;
        };
        virtual ~TTFFont() {
            TTF_CloseFont(font);
            font = NULL;
        }

    protected:
        virtual void setColor(SDL_Color color);
        virtual void getLineSize(std::string text, int *w, int *h);
        virtual void renderLine(SDL_Surface* surface, int x, int y, std::string text);
    
        TTF_Font* font = NULL;
        SDL_Color currentColor;
    };
}

#endif
