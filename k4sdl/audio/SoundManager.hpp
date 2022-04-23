#ifndef K4SDL_SOUND
// Circular-import fix
#include "Sound.hpp"
#endif

#ifndef K4SDL_SOUNDMANAGER
#define K4SDL_SOUNDMANAGER

#include <unordered_map>
#include <SDL_mixer.h>

namespace k4sdl {
    void _soundManagerEndCallback(int channel);

    class SoundManager {
    public:
        static SoundManager* getInstance()
        {
            static SoundManager instance;
            return &instance;
        }

        void stopAllSounds();

        void soundStarted(int channel, Sound* sound);
        void channelEndCallback(int channel);
        void soundDestroyed(Sound* sound);
    private:
        SoundManager() {
            Mix_ChannelFinished(_soundManagerEndCallback);
        }
        std::unordered_map<int, Sound*> endToSound;
    };
}

#endif
