#include "TitleScreen.hpp"

TitleScreen::TitleScreen(k4sdl::SpriteLoader* l_sprLoader, k4sdl::FontLoader* l_fontLoader) : cam(k4sdl::Screen::getRenderer(), 1280, 720) {
    sprLoader = l_sprLoader;
    fontLoader = l_fontLoader;
    cam.zoom = k4sdl::Screen::getZoomFromExpectedSize(1280, 720);
    cam.viewport.x = (1280 - 960) / 2;
    cam.viewport.w = 960;
    gm = k4sdl::GameManager::getInstance();
    inp = gm->getInput();
}

void TitleScreen::run() {
    bool running = true;
    while (running && gm->getRunning()) {
        switch (menu)
        {
        case TitleScreenMenu::MAIN_MENU:
            mainMenu();
            break;
        case TitleScreenMenu::LOAD_SAVE:
            loadSave();
            break;
        default:
            running = false;
            break;
        }
    }
}

void TitleScreen::mainMenu() {
    SDL_Color colorkey;
    colorkey.r = 0;
    colorkey.g = 255;
    colorkey.b = 0;

    k4sdl::Sprite bgSprite;
    sprLoader->load("title/main.png", bgSprite);
    bgSprite.setSize(1280, 720, true, k4sdl::Sprite::SNAP_MIN);

    k4sdl::Sprite senroSprite;
    sprLoader->load("title/senro.png", senroSprite, false, &colorkey);
    senroSprite.setScale(bgSprite.getScale());
    senroSprite.position.y = 720 / 2;
    senroSprite.center.y = k4sdl::Alignment::BTM;

    k4sdl::ButtonSprite startBtn;
    sprLoader->load("title/start.png", startBtn, true, &colorkey);
    startBtn.setScale(bgSprite.getScale());
    startBtn.notPressedTagNum = startBtn.getTagNumByName("off");
    startBtn.pressedTagNum = startBtn.getTagNumByName("on");

    k4sdl::ButtonSprite continueBtn;
    sprLoader->load("title/continue.png", continueBtn, true, &colorkey);
    continueBtn.setScale(bgSprite.getScale());
    continueBtn.position.y += 100;
    if (!Save::getInstance()->existsSave() && false) {
        continueBtn.visible = false;
    }
    continueBtn.notPressedTagNum = continueBtn.getTagNumByName("off");
    continueBtn.pressedTagNum = continueBtn.getTagNumByName("on");

    k4sdl::Sprite trainSprite;
    sprLoader->load("title/train.png", trainSprite, true, &colorkey);
    trainSprite.setScale(bgSprite.getScale());
    trainSprite.position.y = 720 / 2 - 40;
    trainSprite.center.y = k4sdl::Alignment::BTM;
    trainSprite.setTag("gfx");

    k4sdl::Sprite titleSprite;
    sprLoader->load("title/logo.png", titleSprite, false, &colorkey);
    titleSprite.setScale(k4sdl::Vector2<float>(2.5, 2.5));
    titleSprite.position.y = -200;

    float fadeTime = 2.0f;
    float currentFadeTime = 0.0f;

    k4sdl::Sprite fade;
    {
        SDL_Surface* blackSurface = SDL_CreateRGBSurface(0, 1280, 720, 32, 0, 0, 0, 0);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(cam.getRenderer(), blackSurface);
        SDL_FreeSurface(blackSurface);
        fade.setTexture(texture);
    }

    float bgSpeed = 100;
    float senroSpeed = 550;
    // fade in
    while(currentFadeTime < fadeTime && gm->getRunning()) {
        gm->tick();
        currentFadeTime += gm->getDeltaTime();

        scrollSprite(bgSprite, bgSpeed, cam, gm->getDeltaTime());
        scrollSprite(senroSprite, senroSpeed, cam, gm->getDeltaTime());
        trainSprite.animate(gm->getDeltaTime());
        trainSprite.draw(cam);
        titleSprite.draw(cam);
        startBtn.draw(cam);
        continueBtn.draw(cam);

        int value = 255 - (int)(255.0f * currentFadeTime / fadeTime);
        value = (value < 0) ? 0 : value;
        fade.setAlpha(value);
        fade.draw(cam);
    }

    while (gm->getRunning()) {
        gm->tick();

        if (startBtn.getPressed(cam, gm->getDeltaTime())) {
            std::cout << "Start pressed" << std::endl;
            menu = TitleScreenMenu::LOAD_SAVE;
            break;
        }
        else if (continueBtn.getPressed(cam, gm->getDeltaTime())) {
            std::cout << "Continue pressed" << std::endl;
            menu = TitleScreenMenu::LOAD_SAVE;
            break;
        }

        scrollSprite(bgSprite, bgSpeed, cam, gm->getDeltaTime());
        scrollSprite(senroSprite, senroSpeed, cam, gm->getDeltaTime());
        trainSprite.animate(gm->getDeltaTime());
        trainSprite.draw(cam);
        titleSprite.draw(cam);
        startBtn.draw(cam);
        continueBtn.draw(cam);
    }

    currentFadeTime = 0.0f;
    //fade out
    while(currentFadeTime < fadeTime && gm->getRunning()) {
        gm->tick();
        currentFadeTime += gm->getDeltaTime();

        scrollSprite(bgSprite, bgSpeed, cam, gm->getDeltaTime());
        scrollSprite(senroSprite, senroSpeed, cam, gm->getDeltaTime());
        trainSprite.animate(gm->getDeltaTime());
        trainSprite.draw(cam);
        titleSprite.draw(cam);
        startBtn.draw(cam);
        continueBtn.draw(cam);

        int value = (int)(255.0f * currentFadeTime / fadeTime);
        value = (value > 255) ? 255 : value;
        fade.setAlpha(value);
        fade.draw(cam);
    }
}

void TitleScreen::loadSave() {
    while (gm->getRunning()) {
        gm->tick();
        if (inp->getKeyDown(SDL_KeyCode::SDLK_SPACE)) {
            menu = TitleScreenMenu::MAIN_MENU;
            break;
        }
    }
}

void TitleScreen::scrollSprite(k4sdl::Sprite& sprite, float scrollSpeed, k4sdl::Camera &cam, float dt) {
    int spriteW = sprite.getWidth() * sprite.getScale().x;
    sprite.position.x += scrollSpeed * dt;
    if (sprite.position.x < 0)
        sprite.position.x = spriteW;
    else if (sprite.position.x > spriteW)
        sprite.position.x = 0;
    sprite.draw(cam);
    sprite.position.x -= spriteW;
    sprite.draw(cam);
    sprite.position.x += spriteW;
}