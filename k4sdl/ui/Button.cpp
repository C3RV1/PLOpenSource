#include "Button.hpp"

namespace k4sdl {
    bool Button::getPressed(Camera &cam, float dt) {
        hovering = false;
        if (pressed) {
            if (currentPressedCounter <= 0) {
                onNotPress();
                currentPressedCounter = 0;
                return true;
            }
            currentPressedCounter -= dt;
            return false;
        }
        if (getHover(cam)) {
            if (getPress(cam)) {
                onPress();
                currentPressedCounter = pressedCounter;
                return false;
            }
            onHover();
            return false;
        }
        onNotPress();
        return false;
    }
}