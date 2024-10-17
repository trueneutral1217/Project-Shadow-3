#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

SoundManager::~SoundManager() {
    for (auto& sound : sounds) {
        Mix_FreeChunk(sound.second);
    }
    for (auto& track : music) {
        Mix_FreeMusic(track.second);
    }
    Mix_CloseAudio();
}

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

bool SoundManager::loadSound(const std::string& id, const std::string& filename) {
    Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
    if (sound == nullptr) {
        std::cerr << "Failed to load sound: " << filename << " Error: " << Mix_GetError() << std::endl;
        return false;
    }
    sounds[id] = sound;
    return true;
}

void SoundManager::playSound(const std::string& id) {
    if (sounds.find(id) != sounds.end()) {
        Mix_PlayChannel(-1, sounds[id], 0);
    }
}

bool SoundManager::loadMusic(const std::string& id, const std::string& filename) {
    Mix_Music* track = Mix_LoadMUS(filename.c_str());
    if (track == nullptr) {
        std::cerr << "Failed to load music: " << filename << " Error: " << Mix_GetError() << std::endl;
        return false;
    }
    music[id] = track;
    return true;
}

void SoundManager::playMusic(const std::string& id) {
    if (music.find(id) != music.end()) {
        Mix_PlayMusic(music[id], -1);
    }
}

void SoundManager::stopMusic() {
    Mix_HaltMusic();
}
