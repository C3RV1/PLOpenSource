#include "Camera.hpp"

namespace k4sdl {
    void Camera::toScreen(Vector2<float> &point, bool useWorld) {
        Vector2<float> viewportPos(viewport.x, viewport.y);
        Vector2<float> viewportSize(viewport.w, viewport.h);
        if (useWorld)
            point -= worldPosition;
        point += viewportPos;
        point += viewportSize * alignment;
        point *= zoom;
    }

    void Camera::fromScreen(Vector2<float> &point, bool useWorld) {
        Vector2<float> viewportPos(viewport.x, viewport.y);
        Vector2<float> viewportSize(viewport.w, viewport.h);
        point /= zoom;
        point -= viewportSize * alignment;
        point -= viewportPos;
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
        SDL_Rect viewportScaled;
        viewportScaled.x = viewport.x * zoom.x;
        viewportScaled.y = viewport.y * zoom.y;
        viewportScaled.w = viewport.w * zoom.x;
        viewportScaled.h = viewport.h * zoom.y;
        SDL_RenderSetClipRect(renderer, &viewportScaled);
    }

    SDL_Renderer* Camera::getRenderer() {
        return renderer;
    }
}