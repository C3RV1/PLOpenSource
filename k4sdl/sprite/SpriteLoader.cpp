#include "SpriteLoader.hpp"

namespace k4sdl {
    void SpriteLoaderOS::load(std::string path, Sprite &sprite,
                              bool spriteSheet, SDL_Color* colorKey) {
        std::string realPath = basePath + path;
        std::vector<Frame> frameInfo;
        std::vector<Tag> tagInfo;
        SDL_Texture* texture;

        if (spriteSheet) {
            std::string dataFile = realPath + ".dat";
            std::ifstream f(dataFile.c_str(), std::ios::binary);
            if (!f.good()) {
                std::cout << "Error opening spritesheet data. Path: " << dataFile << std::endl;
                return;
            }

            char header[4];
            f.read(header, 4);

            if (memcmp(header, "SPR\x20", 4) != 0) {
                f.close();
                std::cout << "Error on spritesheet data header. Path: " << dataFile << std::endl;
                return;
            }

            uint8_t frameCount, tagCount;
            f.read((char*)&frameCount, 1);
            f.read((char*)&tagCount, 1);

            frameInfo.resize(frameCount);
            for (int i = 0; i < frameCount; i++) {
                f.read((char*)&frameInfo[i].x, 2);
                f.read((char*)&frameInfo[i].y, 2);
                f.read((char*)&frameInfo[i].w, 2);
                f.read((char*)&frameInfo[i].h, 2);
            }

            tagInfo.resize(tagCount);
            for (int i = 0; i < tagCount; i++) {
                f.read((char*)&tagInfo[i].childXOff, 2);
                f.read((char*)&tagInfo[i].childYOff, 2);
                f.read((char*)&tagInfo[i].childTagNum, 2);
                std::getline(f, tagInfo[i].name, '\0');

                uint8_t tagFrameCount;
                f.read((char*)&tagFrameCount, 1);

                tagInfo[i].frames.resize(tagFrameCount);
                for (int j = 0; j < tagFrameCount; j++) {
                    f.read((char*)&tagInfo[i].frames[j], 1);
                }

                tagInfo[i].frameDurations.resize(tagFrameCount);
                for (int j = 0; j < tagFrameCount; j++) {
                    f.read((char*)&tagInfo[i].frameDurations[j], 4);
                }
            }

            f.close();
        }

        SDL_Surface* loadedSurface = IMG_Load(realPath.c_str());
        if (loadedSurface == NULL) {
            std::cout << "Error loading surface. Path: " << realPath << " Error: " << IMG_GetError() << std::endl;
            return;
        }

        if (colorKey != NULL)
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, colorKey->r, colorKey->g, colorKey->b));
        
        texture = SDL_CreateTextureFromSurface(Screen::getRenderer(), loadedSurface);
        SDL_FreeSurface(loadedSurface);
        if (texture == NULL) {
            std::cout << "Error creating texture. Path: " << realPath << " Error: " << SDL_GetError() << std::endl;
            return;
        }

        sprite.loadSprite(texture, frameInfo, tagInfo);
    }
}