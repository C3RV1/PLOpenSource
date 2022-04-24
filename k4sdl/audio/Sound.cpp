#include "Sound.hpp"

namespace k4sdl {
    void Sound::load(Mix_Chunk* l_sndChunk) {
        setChunk(l_sndChunk);
    }

    int Sound::play(int loops, float fadeInTime, int stopAlreadyPlaying) {
        if (sndChunk == NULL)
            return -1;
        if (stopAlreadyPlaying != -2)
            stop(stopAlreadyPlaying);
        int channel = Mix_FadeInChannel(-1, sndChunk, loops, (int)(fadeInTime * 1000.0f));
        if (channel != -1) {
            currentChannels.push_back(channel);
            sndManager->soundStarted(channel, this);
        }
        else
            std::cout << "Error playing sound. Mix_Error: " << Mix_GetError() << std::endl;
        return channel;
    }

    void Sound::stop(int channel, float fadeOutTime) {
        if (channel == -1) {
            std::list<int> channelsToStop(currentChannels);
            for (auto channelToStop : channelsToStop) {
                if (channelToStop != -1)
                    stop(channelToStop, fadeOutTime);
            }
            return;
        }
        if (std::find(currentChannels.begin(), currentChannels.end(), channel) != currentChannels.end()) {
            return;
        }
        if (Mix_GetChunk(channel) == sndChunk)
            Mix_FadeOutChannel(channel, (int)(fadeOutTime * 1000.0f));
    }

    bool Sound::isPlaying() {
        return !currentChannels.empty();
    }

    void Sound::setChunk(Mix_Chunk* l_sndChunk, bool free) {
        stop(-1);
        if (free && sndChunk != NULL)
            Mix_FreeChunk(sndChunk);
        sndChunk = l_sndChunk;
    }

    void Sound::endedOnChannel(int channel) {
        if (std::find(currentChannels.begin(), currentChannels.end(), channel) == currentChannels.end()) {
            return;
        }
        currentChannels.remove(channel);
    }

    Sound::~Sound() {
        sndManager->soundDestroyed(this);
        stop(-1);
        if (sndChunk != NULL)
            Mix_FreeChunk(sndChunk);
    }
}