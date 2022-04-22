#ifndef K4SDL_RENDERABLE
#define K4SDL_RENDERABLE

#include "Vector2/Vector2.hpp"
#include "Alignment.hpp"
#include "Camera.hpp"
#include <SDL.h>

namespace k4sdl {
    class Renderable {
    public:
        Renderable() : center(Alignment::CENTER) {}
        Vector2<float> position;
        bool useWorld = true;
        Vector2<float> center;
        bool visible = true;
        virtual void draw(Camera cam);
        virtual SDL_Rect getScreenRect(Camera cam, SDL_Rect* clip);
        virtual SDL_Rect getWorldRect();
        virtual ~Renderable() {}
    protected:
        void positionToScreen(Camera cam);
        Vector2<float> screenPosition;
    };
}

#endif
