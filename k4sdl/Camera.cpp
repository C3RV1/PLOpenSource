#include "Camera.hpp"

namespace k4sdl {
    void Camera::toScreen(Vector2<float> &point, bool useWorld) {
        Vector2<float> viewportPos(viewport.x, viewport.y);
        Vector2<float> viewportSize(viewport.w, viewport.h);
        if (useWorld)
            point -= worldPosition;
        point *= zoom;
        point += viewportPos;
        point += viewportSize * alignment;
    }

    void Camera::fromScreen(Vector2<float> &point, bool useWorld) {
        Vector2<float> viewportPos(viewport.x, viewport.y);
        Vector2<float> viewportSize(viewport.w, viewport.h);
        point -= viewportSize * alignment;
        point -= viewportPos;
        point /= zoom;
        if (useWorld)
            point += worldPosition;
    }

    SDL_Rect Camera::clipRect(SDL_Rect &rect) {
        SDL_Rect clip;
        clip.x = 0;
        clip.y = 0;
        if (rect.x < viewport.x) {
            clip.x += viewport.x - rect.x;
            rect.x = viewport.x;
            rect.w -= clip.x;
        }
        if (rect.x + rect.w > viewport.x + viewport.w) {
            rect.w -= (rect.x + rect.w) - (viewport.x + viewport.w);
        }
        clip.w = rect.w;
        if (rect.y < viewport.y) {
            clip.y += viewport.y - rect.y;
            rect.y = viewport.y;
            rect.h -= clip.y;
        }
        if (rect.y + rect.h > viewport.y + viewport.h) {
            rect.h -= (rect.y + rect.h) - (viewport.y + viewport.h);
        }
        clip.h = rect.h;
        return clip;
    }

    void Camera::setClip() {
        SDL_RenderSetClipRect(renderer, &viewport);
    }

    SDL_Renderer* Camera::getRenderer() {
        return renderer;
    }
}