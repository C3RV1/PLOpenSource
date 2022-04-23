#include "SoundManager.hpp"

namespace k4sdl {
    void SoundManager::soundStarted(int channel, Sound* sound) {
        endToSound[channel] = sound;
    }

    void SoundManager::channelEndCallback(int channel) {
        if (endToSound.find(channel) != endToSound.end()) {
            Sound* snd = endToSound[channel];
            snd->endedOnChannel(channel);
        }
    }

    void SoundManager::soundDestroyed(Sound* sound) {
        for (auto it = endToSound.begin(); it != endToSound.end();) {
            if (it->second == sound)
                it = endToSound.erase(it);
            else
                ++it;
        }
    }

    void SoundManager::stopAllSounds() {
        Mix_HaltChannel(-1);
    }
    
    void _soundManagerEndCallback(int channel) {
        SoundManager::getInstance()->channelEndCallback(channel);
    }
}