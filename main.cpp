#include "k4sdl/GameManager.hpp"
#include "k4sdl/sprite/Sprite.hpp"
#include "k4sdl/sprite/Text.hpp"
#include "k4sdl/font/FontLoader.hpp"
#include "k4sdl/sprite/SpriteLoader.hpp"
#include <memory>

int main(int argc, char* argv[]) {
    k4sdl::GameManager* gm = k4sdl::GameManager::getInstance();
    k4sdl::GameManagerConfig gmConfig;
    gmConfig.fpsLimit = 144;
    gmConfig.windowName = "Test window";
    gmConfig.setSize(1280, 720);
    gmConfig.logFps = false;
    if (gm->init(gmConfig) != 0) {
        return 0;
    }

    int screenW, screenH;
    k4sdl::Screen::getScreenSize(&screenW, &screenH);
    k4sdl::Camera cam(k4sdl::Screen::getRenderer(), screenW, screenH);

    k4sdl::Input* inp = gm->getInput();

    k4sdl::Sprite sprite;
    k4sdl::Text testText;
    testText.position.set(100, 200);
    testText.center.set(k4sdl::Alignment::CENTER, k4sdl::Alignment::CENTER);
    testText.setAlignment(k4sdl::Alignment::CENTER);

    k4sdl::SpriteLoaderOS spriteLoader("game_data/sprites/");
    k4sdl::FontLoaderOS fontLoader("game_data/fonts/");

    SDL_Color colorKey;
    colorKey.r = 0;
    colorKey.g = 255;
    colorKey.b = 0;
    spriteLoader.load("headphones_play.png", sprite, true, &colorKey);
    sprite.setTag("ON");

    fontLoader.load("prof-layton.ttf", 24, testText);
    std::string testTextStr = "- Hola Ferran que tal tot?\n- Molt bé\n- Vols un pikachu?!\n- SIIIII!";
    testText.setText(testTextStr);

    float moveSpeed = 400;

    while (gm->getRunning()) {
        gm->tick();

        if (inp->getQuit()) {
            gm->exit();
            continue;
        }
        
        if (inp->getKey(SDL_KeyCode::SDLK_s)) {
            sprite.position.y += moveSpeed * gm->getDeltaTime();
        }
        if (inp->getKey(SDL_KeyCode::SDLK_w)) {
            sprite.position.y -= moveSpeed * gm->getDeltaTime();
        }
        if (inp->getKey(SDL_KeyCode::SDLK_d)) {
            testText.setAlignment(std::min(1.0f, testText.getAlignment() + 0.4f * gm->getDeltaTime()));
        }
        if (inp->getKey(SDL_KeyCode::SDLK_a)) {
            testText.setAlignment(std::max(0.0f, testText.getAlignment() - 0.4f * gm->getDeltaTime()));
        }

        sprite.animate(gm->getDeltaTime());
        sprite.rotate(30.0f * gm->getDeltaTime());

        SDL_SetRenderDrawColor(cam.getRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(cam.getRenderer());
        sprite.draw(cam);
        testText.draw(cam);
    }
    return 0;
}