#include "k4sdl/k4sdl.hpp"
#include "game/Save.hpp"
#include "game/TitleScreen/TitleScreen.hpp"


int main(int argc, char* argv[]) {
    k4sdl::GameManager* gm = k4sdl::GameManager::getInstance();
    Save* saveData = Save::getInstance();

    k4sdl::GameManagerConfig gmConfig;
    gmConfig.setSize(1920, 1080);
    gmConfig.fpsLimit = 60;
    gmConfig.fullScreen = true;
    gmConfig.windowName = "Layton Game";
    gmConfig.logFps = false;
    if (gm->init(gmConfig) != 0) {
        std::cout << "Error initializing game." << std::endl;
        return 0;
    }

    k4sdl::SpriteLoaderOS sprLoader("game_data/sprites/");
    k4sdl::FontLoaderOS fntLoader("game_data/fonts/");

    gm->tick();
    while (gm->getRunning()) {
        switch (saveData->gameMode) {
        case GameMode::TITLESCREEN: {
            TitleScreen titleScreen(&sprLoader, &fntLoader);
            titleScreen.run();
            break;
        }
        default:
            saveData->gameMode = GameMode::TITLESCREEN;
            break;
        }
    }

    gm->exit();
    return 0;
}
