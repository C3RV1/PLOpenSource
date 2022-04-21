#include "Renderable.hpp"

namespace k4sdl {
    void Renderable::draw(Camera cam) {
        cam.setClip();
        positionToScreen(cam);
    }

    SDL_Rect Renderable::getScreenRect(Camera cam, SDL_Rect* clip) {
        SDL_Rect none;
        return none;
    }

    SDL_Rect Renderable::getWorldRect() {
        SDL_Rect none;
        return none;
    }

    void Renderable::positionToScreen(Camera cam) {
        screenPosition = position;
        cam.toScreen(screenPosition);
    }
}