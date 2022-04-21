#include "Input.hpp"

namespace k4sdl {
    void Input::tickEvents() {
        SDL_Event e;
        keyUpdated.clear();
        mouseUpdated.clear();
        mouseMoved = false;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                SDL_Keycode key = e.key.keysym.sym;
                if (std::find(keyUpdated.begin(), keyUpdated.end(), key) == keyUpdated.end()) {
                    keyUpdated.push_back(key);
                }
                keyDown[key] = true;
            }
            else if (e.type == SDL_KEYUP) {
                SDL_Keycode key = e.key.keysym.sym;
                if (std::find(keyUpdated.begin(), keyUpdated.end(), key) == keyUpdated.end()) {
                    keyUpdated.push_back(key);
                }
                keyDown[key] = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int button = e.button.button;
                if (std::find(mouseUpdated.begin(), mouseUpdated.end(), button) == mouseUpdated.end()) {
                    mouseUpdated.push_back(button);
                }
                mouseDown[button] = true;
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                int button = e.button.button;
                if (std::find(mouseUpdated.begin(), mouseUpdated.end(), button) == mouseUpdated.end()) {
                    mouseUpdated.push_back(button);
                }
                mouseDown[button] = false;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                mouseX = e.motion.x;
                mouseY = e.motion.y;
                mouseMotionX = e.motion.xrel;
                mouseMotionY = e.motion.yrel;
                mouseMoved = true;
            }
        }
    }

    bool Input::getKeyDown(SDL_Keycode key) {
        if (std::find(keyUpdated.begin(), keyUpdated.end(), key) == keyUpdated.end())
            return false;
        return keyDown[key];
    }

    bool Input::getKeyUp(SDL_Keycode key) {
        if (std::find(keyUpdated.begin(), keyUpdated.end(), key) == keyUpdated.end())
            return false;
        return !keyDown[key];
    }

    bool Input::getKey(SDL_Keycode key) {
        if (keyDown.find(key) == keyDown.end())
            return false;
        return keyDown[key];
    }

    bool Input::getMouseDown(int button) {
        if (std::find(mouseUpdated.begin(), mouseUpdated.end(), button) == mouseUpdated.end())
            return false;
        return mouseDown[button];
    }

    bool Input::getMouseUp(int button) {
        if (std::find(mouseUpdated.begin(), mouseUpdated.end(), button) == mouseUpdated.end())
            return false;
        return !mouseDown[button];
    }

    bool Input::getMouse(int button) {
        if (mouseDown.find(button) == mouseDown.end())
            return false;
        return mouseDown[button];
    }

    void Input::getMousePos(int *x, int *y) {
        if (x != NULL)
            *x = mouseX;
        if (y != NULL)
            *y = mouseY;
    }

    void Input::getMouseRel(int *xRel, int *yRel) {
        if (xRel != NULL)
            *xRel = mouseMotionX;
        if (yRel != NULL)
            *yRel = mouseMotionY;
    }

    bool Input::getMouseMoved() {
        return mouseMoved;
    }

    bool Input::getQuit() {
        return quit;
    }
}