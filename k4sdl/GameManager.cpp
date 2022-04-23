#include "GameManager.hpp"

namespace k4sdl {
    int GameManager::init(GameManagerConfig gmConfig) {
        if (m_initiated)
            return -1;
        m_initiated = true;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not initialize. SDL_Error: " << SDL_GetError() << std::endl;
            return 1;
        }

        int imgFlags = IMG_INIT_PNG;
        if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
            std::cout << "SDL_Image could not initialize. IMG_Error: " << IMG_GetError() << std::endl;
            return 2;
        }

        if (TTF_Init() < 0) {
            std::cout << "SDL_TTF could not initialize. TTF_Error: " << TTF_GetError() << std::endl;
            return 3;
        }

        int flags = SDL_WINDOW_SHOWN;
        if (gmConfig.fullScreen) {
            flags |= SDL_WINDOW_FULLSCREEN;
        }

        int screenRes = Screen::newScreen(gmConfig.screenX, gmConfig.screenY, gmConfig.screenWidth, gmConfig.screenHeight, flags, gmConfig.windowName);
        if (screenRes != 0) {
            std::cout << "Screen could not initialize." << std::endl;
            return 4;
        }

        m_lastTick = SDL_GetTicks();
        m_running = true;
        m_deltaTime = 1.0f;
        m_logFps = gmConfig.logFps;
        if (gmConfig.fpsLimit > 0)
            m_ticksPerFrame = 1000 / gmConfig.fpsLimit;
        else
            m_ticksPerFrame = 0;

        return 0;
    }

    void GameManager::tick() {
        if (!m_running)
            return;
        SDL_RenderPresent(Screen::getRenderer());
        int ticks = SDL_GetTicks() - m_lastTick;
        if (ticks < m_ticksPerFrame) {
            SDL_Delay(m_ticksPerFrame - ticks);
            ticks = SDL_GetTicks() - m_lastTick;
        }
        m_deltaTime = (float)ticks / 1000.0f;

        if (m_logFps)
            std::cout << "FPS: " << (int)(1.0f/m_deltaTime) << std::endl;

        m_lastTick = SDL_GetTicks();
        m_inputManager.tickEvents();
        if (m_inputManager.getQuit() || m_inputManager.getKeyDown(SDL_KeyCode::SDLK_ESCAPE))
            exit();
    }

    void GameManager::exit() {
        Screen::exit();
        IMG_Quit();
        SDL_Quit();
        m_running = false;
    }

    float GameManager::getDeltaTime() {
        return m_deltaTime;
    }

    bool GameManager::getRunning() {
        return m_running;
    }

    void GameManagerConfig::setSize(int l_screenWidth, int l_screenHeight) {
        screenWidth = l_screenWidth;
        screenHeight = l_screenHeight;
    }

    Input* GameManager::getInput() {
        return &m_inputManager;
    }
}