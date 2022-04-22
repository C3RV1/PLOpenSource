#ifndef K4SDL_CAMERA
#define K4SDL_CAMERA

#include "Vector2/Vector2.hpp"
#include "Alignment.hpp"
#include <SDL.h>

namespace k4sdl {
    class Camera {
    public:
        Camera(SDL_Renderer* l_renderer, int width, int height) : renderer(l_renderer),
                                                                  alignment(Alignment::CENTER),
                                                                  zoom(1) {
            viewport.x = 0;
            viewport.y = 0;
            viewport.w = width;
            viewport.h = height;
        }
        Vector2<float> worldPosition;
        Vector2<float> alignment;
        Vector2<float> zoom;
        SDL_Rect viewport;

        void toScreen(Vector2<float> &point, bool useWorld = true);
        void fromScreen(Vector2<float> &point, bool useWorld = true);
        SDL_Rect clipRect(SDL_Rect &rect);
        void setClip();
        SDL_Renderer* getRenderer();
    private:
        SDL_Renderer* renderer;
    };
}

#endif
