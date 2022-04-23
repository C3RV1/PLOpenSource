#include "Sound.hpp"

namespace k4sdl {
    void Sound::load(Mix_Chunk* l_sndChunk) {
        setChunk(l_sndChunk);
    }

    int Sound::play(int loops, int stopAlreadyPlaying) {
        if (sndChunk == NULL)
            return -1;
        if (stopAlreadyPlaying != -2)
            stop(stopAlreadyPlaying);
        int channel = Mix_PlayChannel(-1, sndChunk, loops);
        if (channel != -1) {
            currentChannels.push_back(channel);
            sndManager->soundStarted(channel, this);
        }
        else
            std::cout << "Error playing sound. Mix_Error: " << Mix_GetError() << std::endl;
        return channel;
    }

    void Sound::stop(int channel) {
        if (channel == -1) {
            std::list<int> channelsToStop(currentChannels);
            for (auto channelToStop : channelsToStop) {
                if (channelToStop != -1)
                    stop(channelToStop);
            }
            return;
        }
        if (std::find(currentChannels.begin(), currentChannels.end(), channel) == currentChannels.end()) {
            currentChannels.remove(channel);
        }
        if (Mix_GetChunk(channel) == sndChunk)
            Mix_HaltChannel(channel);
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