#ifndef GAME_TITLESCREEN
#define GAME_TITLESCREEN

#include "../../k4sdl/k4sdl.hpp"
#include "../Save.hpp"
#include <memory>

enum TitleScreenMenu {
    MAIN_MENU,
    LOAD_SAVE,
    RETURN
};

class TitleScreen {
public:
    TitleScreen(k4sdl::SpriteLoader* l_sprLoader, k4sdl::FontLoader* l_fontLoader);
    void run();
    void mainMenu();
    void loadSave();
private:
    void scrollSprite(k4sdl::Sprite& sprite, float scrollSpeed, k4sdl::Camera &cam, float dt);
    k4sdl::GameManager* gm;
    k4sdl::Input* inp;
    k4sdl::SoundManager* sndManager;
    k4sdl::SpriteLoader* sprLoader;
    k4sdl::FontLoader* fontLoader;
    k4sdl::Camera cam;
    TitleScreenMenu menu = MAIN_MENU;
};

#endif
