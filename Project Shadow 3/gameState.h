#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include <fstream>
#include <map>
#include <vector>
#include <string>

class GameState {
public:
    GameState(const std::string& saveFile);
    void save(const Player& player);
    void load(Player& player);

    void saveTileMap(const std::map<int, std::map<int, int>>& tileMap);
    void loadTileMap(std::map<int, std::map<int, int>>& tileMap);

private:
    std::string saveFilePath;
};

#endif // GAMESTATE_H
