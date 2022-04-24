#include "k4sdl/k4sdl.hpp"
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
    k4sdl::Camera cam(k4sdl::Screen::getRenderer(), 1280, 720);
    cam.viewport.x = screenW / 4 - 20;
    cam.viewport.y = screenH / 4 - 20;
    cam.viewport.w = screenW / 2 + 40;
    cam.viewport.h = screenH / 2 + 40;

    k4sdl::Input* inp = k4sdl::Input::getInstance();

    k4sdl::Sound testSound;

    k4sdl::Sprite sprite;

    k4sdl::ButtonSprite testBtn;
    testBtn.position.set(-100, -200);

    k4sdl::Text testText;
    testText.position.set(100, 200);
    testText.setAlignment(k4sdl::Alignment::CENTER);

    k4sdl::SpriteLoaderOS spriteLoader("game_data/sprites/");
    k4sdl::FontLoaderOS fontLoader("game_data/fonts/");
    k4sdl::SoundLoaderOS soundLoader("game_data/audio/");

    SDL_Color colorKey;
    colorKey.r = 0;
    colorKey.g = 255;
    colorKey.b = 0;
    spriteLoader.load("event_chr/chr1.arc.png", sprite, true, &colorKey);
    sprite.setTagByNum(0);
    sprite.setAlpha(127);

    fontLoader.load("prof-layton.ttf", 24, testText);
    std::string testTextStr = "Any test string\nReally serves this purpose\nQuite well";
    testText.setText(testTextStr);

    spriteLoader.load("done_btn.png", testBtn, true, &colorKey);
    testBtn.notPressedTagNum = testBtn.getTagNumByName("Idle");
    testBtn.pressedTagNum = testBtn.getTagNumByName("Press");
    testBtn.hoverTagNum = testBtn.getTagNumByName("Hover");

    soundLoader.load("ST_IMA.wav", testSound);

    float moveSpeed = 400;
    float rotateSpeed = 45;

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
        if (inp->getKey(SDL_KeyCode::SDLK_q)) {
            sprite.rotate(-rotateSpeed * gm->getDeltaTime());
        }
        if (inp->getKey(SDL_KeyCode::SDLK_e)) {
            sprite.rotate(rotateSpeed * gm->getDeltaTime());
        }
        if (inp->getKeyDown(SDL_KeyCode::SDLK_SPACE)) {
            sprite.setTagByNum((sprite.getTagNum() + 1) % sprite.getTagCount());
            std::cout << "Set tag " << sprite.getTagName() << std::endl;
        }
        if (inp->getKeyDown(SDL_KeyCode::SDLK_p)) {
            // Play without stopping already playing
            // Set , -1 to stop all current channels playing this sound
            // before playing it again
            testSound.play(0 /*, -1*/);
        }

        if (testBtn.getPressed(cam, gm->getDeltaTime())) {
            std::cout << "Test button pressed" << std::endl;
        }

        sprite.animate(gm->getDeltaTime());

        SDL_SetRenderDrawColor(cam.getRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(cam.getRenderer());
        sprite.draw(cam);
        testText.draw(cam);
        testBtn.draw(cam);
    }
    return 0;
}