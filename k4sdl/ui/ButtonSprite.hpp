#ifndef K4SDL_BUTTONSPIRTE
#define K4SDL_BUTTONSPIRTE

#include "Button.hpp"
#include "../sprite/Sprite.hpp"

namespace k4sdl {
    class ButtonSprite : public Sprite, public Button {
    public:
        int pressedTagNum = -1;
        int hoverTagNum = -1;
        int notPressedTagNum = -1;

    protected:
        virtual void onNotPress();
        virtual void onPress();
        virtual void onHover();
        virtual bool getHover(Camera &cam);
        virtual bool getPress(Camera &cam);
    };
}

#endif
