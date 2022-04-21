#include "Font.hpp"

namespace k4sdl {
    SDL_Texture* Font::render(std::string text, SDL_Color color, SDL_Color* bgColor, int lineSpacing = 0, float hAlign = Alignment::LEFT) {
        int lineCount = std::count(text.begin(), text.end(), '\n');
        std::vector<std::string> lines(lineCount + 1);

        std::istringstream lineStream(text);
        for (int i = 0; std::getline(lineStream, lines[i]); i++);

        setColor(color);

        int surfWidth = 0, surfHeight = 0;
        std::vector<int> widths(lineCount), heights(lineCount);
        for (int i = 0; i < lineCount; i++) {
            getLineSize(lines[i], &widths[i], &heights[i]);
            if (widths[i] > surfWidth)
                surfWidth = widths[i];
            surfHeight += heights[i];
        }

        surfHeight += lineSpacing * (lineCount - 1);

        SDL_Surface *surface;
        Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
#else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
#endif

        surface = SDL_CreateRGBSurface(0, surfWidth, surfHeight, 32,
                                       rmask, gmask, bmask, amask);
        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
        if (surface == NULL) {
            std::cout << "SDL_CreateRGBSurface failed: " << SDL_GetError() << std::endl;
            return NULL;
        }

        if (bgColor != NULL)
            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, bgColor->r, bgColor->g, bgColor->b, bgColor->a));
        else
            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
        
        int currentY = 0;
        for (int i = 0; i < lineCount; i++) {
            int posX = (int)floor((float)(surfWidth - widths[i]) * hAlign);
            renderLine(surface, posX, currentY, lines[i]);
            currentY += heights[i] + lineSpacing;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(Screen::getRenderer(), surface);
        SDL_FreeSurface(surface);
        if (texture == NULL) {
            std::cout << "Error creating texture. Error: " << SDL_GetError() << std::endl;
            return NULL;
        }

        return texture;
    }

    void TTFFont::setColor(SDL_Color color) {
        currentColor = color;
    }

    void TTFFont::getLineSize(std::string text, int *w, int *h) {
        TTF_SizeText(font, text.c_str(), w, h);
    }

    void TTFFont::renderLine(SDL_Surface* surface, int x, int y, std::string text) {
        SDL_Surface* renderedText = TTF_RenderText_Solid(font, text.c_str(), currentColor);
        if (renderedText == NULL) {
            std::cout << "Error rendering text. Error: " << TTF_GetError() << std::endl;
            return;
        }
        SDL_Rect dstRect;
        dstRect.x = x;
        dstRect.y = y;
        dstRect.w = renderedText->w;
        dstRect.h = renderedText->h;
        SDL_BlitSurface(renderedText, NULL, surface, &dstRect);
        SDL_FreeSurface(renderedText);
        renderedText = NULL;
    }
}