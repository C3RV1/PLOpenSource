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

    void SoundManager::loadMusic(std::string path) {
        Mix_Music* l_music = Mix_LoadMUS(path.c_str());
        if (l_music == NULL) {
            std::cout << "Music couldn't be loaded. Path: " << path << " Mix_Error: " << Mix_GetError() << std::endl;
            return;
        }
        stopMusic();
        Mix_FreeMusic(music);
        music = l_music;
    }

    void SoundManager::playMusic(int loops, float fadeInTime) {
        stopMusic();
        Mix_FadeInMusic(music, loops, (int)(fadeInTime * 1000.0f));
    }

    void SoundManager::stopMusic(float fadeOutTime) {
        if (playingMusic()) {
            Mix_FadeOutMusic((int)(fadeOutTime * 1000.0f));
        }
    }

    bool SoundManager::playingMusic() {
        return Mix_PlayingMusic() == 1;
    }

    void SoundManager::pauseMusic() {
        Mix_PauseMusic();
    }

    void SoundManager::resumeMusic() {
        Mix_ResumeMusic();
    }

    bool SoundManager::pausedMusic() {
        return Mix_PausedMusic() == 1;
    }

    void SoundManager::stopAllSounds() {
        Mix_HaltChannel(-1);
    }
    
    void _soundManagerEndCallback(int channel) {
        SoundManager::getInstance()->channelEndCallback(channel);
    }
}