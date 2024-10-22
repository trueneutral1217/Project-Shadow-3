#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>
#include <string>
#include <map>
#include <vector>

class SoundManager {
public:
    static SoundManager& getInstance();
    bool loadSound(const std::string& id, const std::string& filename);
    void playSound(const std::string& id);
    void playMusic(const std::string& id);
    void stopMusic();
    ~SoundManager();

    bool loadMusic(const std::string& id, const std::string& filename);
    void playRandomMusic();
private:
    SoundManager();
    std::map<std::string, Mix_Chunk*> sounds;
    std::map<std::string, Mix_Music*> music;
    std::vector<std::string> musicIds;
    void onMusicEnd();
    static void musicFinishedCallback();
};

#endif // SOUNDMANAGER_H
