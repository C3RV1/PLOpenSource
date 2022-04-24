#ifndef K4SDL_SOUND
#define K4SDL_SOUND

namespace k4sdl {
    class Sound;
}

#include <SDL_mixer.h>
#include <list>
#include <iostream>
#include <algorithm>
#include "SoundManager.hpp"

namespace k4sdl {
    class Sound {
    public:
        Sound() : sndManager(SoundManager::getInstance()) {}
        void load(Mix_Chunk* l_sndChunk);
        int play(int loops, float fadeInTime = 0.0f, int stopAlreadyPlaying=-2);
        void stop(int channel=-1, float fadeOutTime = 0.0f);
        bool isPlaying();
        void setChunk(Mix_Chunk* l_sndChunk, bool free=true);
        void endedOnChannel(int channel);
        ~Sound();
    private:
        Mix_Chunk* sndChunk = NULL;
        std::list<int> currentChannels;
        SoundManager* sndManager;
    };
}

#endif
