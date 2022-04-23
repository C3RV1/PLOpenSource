#ifndef K4SDL_BUTTON
#define K4SDL_BUTTON

#include "../GameManager.hpp"
#include "../Camera.hpp"

namespace k4sdl {
    class Button {
    public:
        Button(float l_pressedCounter = 0.1f) : pressedCounter(l_pressedCounter) {
            inp = GameManager::getInstance()->getInput();
        }
        virtual bool getPressed(Camera &cam, float dt) final;
        float pressedCounter = 0.1f;
    protected:
        virtual void onNotPress() {
            pressed = false;
        }
        virtual void onPress() {
            pressed = true;
        }
        virtual void onHover() {
            hovering = true;
        }
        virtual bool getHover(Camera &cam) {
            return false;
        }
        virtual bool getPress(Camera &cam) {
            return false;
        }
        float currentPressedCounter = 0.0f;
        bool pressed = false;
        bool hovering = false;
        Input* inp;
    };
}

#endif
