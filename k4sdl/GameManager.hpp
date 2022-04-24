#ifndef K4SDL_GM
#define K4SDL_GM

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Screen.hpp"
#include "input/Input.hpp"
#include <iostream>

namespace k4sdl {
    class GameManagerConfig {
    public:
        int screenX = SDL_WINDOWPOS_UNDEFINED, screenY = SDL_WINDOWPOS_UNDEFINED, screenWidth = 0, screenHeight = 0;
        bool fullScreen = false;
        bool logFps = true;
        int fpsLimit = 60;
        std::string windowName = "";

        void setSize(int l_screenWidth, int l_screenHeight);
    };

    class GameManager {
    public:
        static GameManager* getInstance()
        {
            static GameManager instance;
            return &instance;
        }

        int init(GameManagerConfig gmConfig);
        void exit();
        void tick();
        float getDeltaTime();
        bool getRunning();

        GameManager(GameManager const&) = delete;
        void operator=(GameManager const&) = delete;

        float m_timeScale = 1.0f;
    private:
        GameManager() {
            m_inputManager = Input::getInstance();
        }

        bool m_initiated = false;
        bool m_running = false;
        float m_deltaTime = 0.0f;
        int m_lastTick = 0;
        bool m_logFps;
        int m_ticksPerFrame;

        Input* m_inputManager;
    };
}

#endif
