#ifndef K4SDL_TEXT
#define K4SDL_TEXT

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Sprite.hpp"
#include "../font/Font.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include "../Alignment.hpp"

namespace k4sdl {
    class Text : public Sprite {
    public:
        Text() {
            text = "";
            color.r = 255;
            color.g = 255;
            color.b = 255;
            color.a = 255;
        }
        void setFont(std::unique_ptr<Font> l_font);
        void setText(std::string l_text);
        std::string getText();
        void setBgColor(std::unique_ptr<SDL_Color> l_bgColor);
        SDL_Color getBgColor();
        void setLineSpacing(int l_lineSpacing);
        int getLineSpacing();
        void setAlignment(float l_alignment);
        float getAlignment();
        virtual void draw(Camera &cam);

    private:
        std::unique_ptr<Font> font = nullptr;
        std::string text;
        SDL_Color color;
        std::unique_ptr<SDL_Color> bgColor = nullptr;
        int lineSpacing = 0;
        float alignment = Alignment::LEFT;
        bool renderNeeded = false;
        void render();
    };
}

#endif