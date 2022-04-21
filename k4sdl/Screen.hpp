#ifndef K4SDL_SCREEN
#define K4SDL_SCREEN

#include <SDL.h>
#include <string>
#include <iostream>

namespace k4sdl {
    class Screen {
    public:
        static int newScreen(int screenX, int screenY, int screenWidth, int screenHeight, int flags, std::string name);
        static SDL_Window* getWindow();
        static SDL_Renderer* getRenderer();
        static void exit();
        static void getScreenSize(int *w, int *h);
    private:
        static SDL_Window* m_window;
        static SDL_Renderer* m_renderer;
        static int m_screenWidth;
        static int m_screenHeight;
    };
}

#endif
