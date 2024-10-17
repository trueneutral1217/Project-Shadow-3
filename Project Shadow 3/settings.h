#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings {
public:
    Settings(const std::string& settingsFile);
    bool loadSettings();
    bool saveSettings();
    int getVolume() const;
    void setVolume(int volume);
    int getResolutionWidth() const;
    void setResolutionWidth(int width);
    int getResolutionHeight() const;
    void setResolutionHeight(int height);

private:
    std::string settingsFile;
    int volume;
    int resolutionWidth;
    int resolutionHeight;
};

#endif // SETTINGS_H
