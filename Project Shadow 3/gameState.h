#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include <fstream>

class GameState {
public:
    GameState(const std::string& saveFile);
    void save(const Player& player);
    void load(Player& player);

private:
    std::string saveFilePath;
};

#endif // GAMESTATE_H
