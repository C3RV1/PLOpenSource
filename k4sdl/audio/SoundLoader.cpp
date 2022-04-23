#include "SoundLoader.hpp"

namespace k4sdl {
    void SoundLoaderOS::load(std::string path, Sound &sound) {
        std::string realPath = basePath + path;
        
        std::string extension = realPath.substr(realPath.find_last_of("."));
        if (extension.compare(".wav") == 0) {
            Mix_Chunk* soundChunk = Mix_LoadWAV(realPath.c_str());
            if (soundChunk == NULL) {
                std::cout << "Error opening wav. Path: " << realPath << " Mix_Error: " << Mix_GetError() << std::endl;
                return;
            }
            sound.load(soundChunk);
            return;
        }
        std::cout << "Error loading sound. Path: " << realPath << " (Extension " << extension << " not supported.)" << std::endl;
    }
}