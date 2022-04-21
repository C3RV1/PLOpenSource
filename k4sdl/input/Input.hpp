#ifndef K4SDL_INPUT
#define K4SDL_INPUT

#include <map>
#include <list>
#include <algorithm>
#include <SDL.h>

namespace k4sdl {
    class Input {
    public:
        void tickEvents();
        bool getKeyDown(SDL_Keycode key);
        bool getKeyUp(SDL_Keycode key);
        bool getKey(SDL_Keycode key);
        bool getMouseDown(int button);
        bool getMouseUp(int button);
        bool getMouse(int button);
        void getMousePos(int *x, int *y);
        void getMouseRel(int *xRel, int *yRel);
        bool getMouseMoved();
        bool getQuit();
    private:
        std::map<SDL_Keycode, bool> keyDown;
        std::list<SDL_Keycode> keyUpdated;

        std::map<int, bool> mouseDown;
        std::list<int> mouseUpdated;

        bool quit = false;
        bool mouseMoved = false;
        int mouseX = 0, mouseY = 0;
        int mouseMotionX = 0, mouseMotionY = 0;
    };
}

#endif
