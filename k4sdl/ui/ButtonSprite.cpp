#include "ButtonSprite.hpp"

namespace k4sdl {
    void ButtonSprite::onNotPress() {
        Button::onNotPress();
        this->setTagByNum(notPressedTagNum);
    }
    
    void ButtonSprite::onPress() {
        Button::onPress();
        this->setTagByNum(pressedTagNum);
    }

    void ButtonSprite::onHover() {
        Button::onHover();
        this->setTagByNum(hoverTagNum);
    }

    bool ButtonSprite::getHover(Camera &cam) {
        if (!visible)
            return false;
        int mx, my;
        inp->getMousePos(&mx, &my);
        SDL_Rect sr = this->getScreenRect(cam, NULL);
        return (mx > sr.x && my > sr.y && mx < sr.x + sr.w && my < sr.y + sr.h);
    }

    bool ButtonSprite::getPress(Camera &cam) {
        if (!visible)
            return false;
        return inp->getMouseDown(1);
    }
}