#include "Settings.h"
#include <fstream>
#include <iostream>

Settings::Settings(const std::string& settingsFile)
    : settingsFile(settingsFile), volume(100), resolutionWidth(256), resolutionHeight(192) {}

bool Settings::loadSettings() {
    std::ifstream file(settingsFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open settings file: " << settingsFile << std::endl;
        return false;
    }

    file >> volume;
    file >> resolutionWidth;
    file >> resolutionHeight;

    file.close();
    return true;
}

bool Settings::saveSettings() {
    std::ofstream file(settingsFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open settings file: " << settingsFile << std::endl;
        return false;
    }

    file << volume << std::endl;
    file << resolutionWidth << std::endl;
    file << resolutionHeight << std::endl;

    file.close();
    return true;
}

int Settings::getVolume() const {
    return volume;
}

void Settings::setVolume(int volume) {
    this->volume = volume;
}

int Settings::getResolutionWidth() const {
    return resolutionWidth;
}

void Settings::setResolutionWidth(int width) {
    resolutionWidth = width;
}

int Settings::getResolutionHeight() const {
    return resolutionHeight;
}

void Settings::setResolutionHeight(int height) {
    resolutionHeight = height;
}
