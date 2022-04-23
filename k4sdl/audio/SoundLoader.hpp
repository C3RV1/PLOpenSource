#ifndef K4SDL_SOUNDLOADER
#define K4SDL_SOUNDLOADER

#include <string>
#include "Sound.hpp"
#include <SDL_mixer.h>

namespace k4sdl {
    class SoundLoader {
    public:
        virtual void load(std::string path, Sound& sound) = 0;
    };

    class SoundLoaderOS : public SoundLoader {
    public:
        SoundLoaderOS(std::string l_basePath = "") : basePath(l_basePath) {};
        virtual void load(std::string path, Sound &sound);
    private:
        std::string basePath;
    };
}

#endif
