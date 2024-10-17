#include "GameState.h"
#include <iostream>

GameState::GameState(const std::string& saveFile) : saveFilePath(saveFile) {}

void GameState::save(const Player& player) {
    std::ofstream outFile(saveFilePath, std::ios::out | std::ios::binary);
    if (outFile.is_open()) {
        // Save player position
        outFile.write((char*)&player.xPos, sizeof(player.xPos));
        outFile.write((char*)&player.yPos, sizeof(player.yPos));
        
        // Save inventory
        int itemCount = player.inventory.items.size();
        outFile.write((char*)&itemCount, sizeof(itemCount));
        for (const auto& item : player.inventory.items) {
            int length = item.size();
            outFile.write((char*)&length, sizeof(length));
            outFile.write(item.c_str(), length);
        }
        
        outFile.close();
    } else {
        std::cerr << "Unable to open save file for writing!" << std::endl;
    }
}

void GameState::load(Player& player) {
    std::ifstream inFile(saveFilePath, std::ios::in | std::ios::binary);
    if (inFile.is_open()) {
        // Load player position
        inFile.read((char*)&player.xPos, sizeof(player.xPos));
        inFile.read((char*)&player.yPos, sizeof(player.yPos));
        player.destRect.x = player.xPos;
        player.destRect.y = player.yPos;
        
        // Load inventory
        int itemCount;
        inFile.read((char*)&itemCount, sizeof(itemCount));
        player.inventory.items.clear();
        for (int i = 0; i < itemCount; ++i) {
            int length;
            inFile.read((char*)&length, sizeof(length));
            char* buffer = new char[length];
            inFile.read(buffer, length);
            player.inventory.addItem(std::string(buffer, length));
            delete[] buffer;
        }
        
        inFile.close();
    } else {
        std::cerr << "Unable to open save file for reading!" << std::endl;
    }
}
