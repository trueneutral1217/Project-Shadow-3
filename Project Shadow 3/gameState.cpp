#include "GameState.h"
#include <iostream>

GameState::GameState(const std::string& saveFile) : saveFilePath(saveFile) {}
//  std::vector<std::vector<int>>& map
void GameState::save(const Player& player) {
    std::cout<<"\n saving player";
    std::ofstream outFile(saveFilePath, std::ios::out | std::ios::binary);
    if (outFile.is_open()) {
        // Save player position
        outFile.write((char*)&player.xPos, sizeof(player.xPos));
        outFile.write((char*)&player.yPos, sizeof(player.yPos));

        /*
        // Save inventory
        int itemCount = player.inventory.size();
        outFile.write((char*)&itemCount, sizeof(itemCount));
        for (const auto& item : player.inventory) {
            int length = item.size();
            outFile.write((char*)&length, sizeof(length));
            outFile.write(iventory.c_str(), length);
        }*/

        outFile.close();
    } else {
        std::cerr << "Unable to open save file for writing!" << std::endl;
    }
}

void GameState::load(Player& player) {
    std::cout<<"\n loading player";
    std::ifstream inFile(saveFilePath, std::ios::in | std::ios::binary);
    if (inFile.is_open()) {
        // Load player position
        inFile.read((char*)&player.xPos, sizeof(player.xPos));
        inFile.read((char*)&player.yPos, sizeof(player.yPos));
        player.destRect.x = player.xPos;
        player.destRect.y = player.yPos;

        /*
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
        }*/

        inFile.close();
    } else {
        std::cerr << "Unable to open save file for reading!" << std::endl;
    }
}

void GameState::saveTileMap(const std::map<int, std::map<int, int>>& tileMap) {
    std::cout<<"\n saving Tilemap";
    std::ofstream out("tileMap.dat", std::ios::binary);
    if (!out.is_open()) {
        return;
    }
    for (const auto& row : tileMap) {
        int rowKey = row.first;
        out.write(reinterpret_cast<const char*>(&rowKey), sizeof(rowKey));
        int rowSize = row.second.size();
        out.write(reinterpret_cast<const char*>(&rowSize), sizeof(rowSize));
        for (const auto& cell : row.second) {
            int colKey = cell.first;
            int tileValue = cell.second;
            out.write(reinterpret_cast<const char*>(&colKey), sizeof(colKey));
            out.write(reinterpret_cast<const char*>(&tileValue), sizeof(tileValue));
        }
    }
    out.close();
}

void GameState::loadTileMap(std::map<int, std::map<int, int>>& tileMap) {
    std::cout<<"\n loading tilemap";
    std::ifstream in("tileMap.dat", std::ios::binary);
    if (!in.is_open()) {
        return;
    }
    tileMap.clear();
    while (in) {
        int rowKey;
        in.read(reinterpret_cast<char*>(&rowKey), sizeof(rowKey));
        if (!in) break;
        int rowSize;
        in.read(reinterpret_cast<char*>(&rowSize), sizeof(rowSize));
        std::map<int, int> row;
        for (int i = 0; i < rowSize; ++i) {
            int colKey;
            int tileValue;
            in.read(reinterpret_cast<char*>(&colKey), sizeof(colKey));
            in.read(reinterpret_cast<char*>(&tileValue), sizeof(tileValue));
            row[colKey] = tileValue;
        }
        tileMap[rowKey] = row;
    }
    in.close();
}
