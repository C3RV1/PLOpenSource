#include "Screen.hpp"

namespace k4sdl {
    SDL_Window* Screen::m_window = NULL;
    SDL_Renderer* Screen::m_renderer = NULL;
    int Screen::m_screenWidth = 0;
    int Screen::m_screenHeight = 0;

    int Screen::newScreen(int screenX, int screenY, int screenWidth, int screenHeight, int flags, std::string name) {
        m_window = SDL_CreateWindow(name.c_str(), screenX, screenY, screenWidth, screenHeight, flags);
        if (m_window == NULL) {
            std::cout << "Window could not be created. SDL_Error: " << SDL_GetError() << std::endl;
            return 1;
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        if (m_renderer == NULL) {
            std::cout << "Renderer could not be created. SDL_Error: " << SDL_GetError() << std::endl;
            return 2;
        }

        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;

        return 0;
    }

    SDL_Window* Screen::getWindow() {
        return m_window;
    }

    SDL_Renderer* Screen::getRenderer() {
        return m_renderer;
    }

    void Screen::exit() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    void Screen::getScreenSize(int *w, int *h) {
        if (w != NULL)
            *w = m_screenWidth;
        if (h != NULL)
            *h = m_screenHeight;
    }
}