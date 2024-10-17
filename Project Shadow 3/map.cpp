#include "Map.h"
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>

Map::Map(const char* mapFile, SDL_Renderer* renderer) {
    std::ifstream file(mapFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << mapFile << std::endl;
        return;
    }

    file >> width >> height;
    map = new int*[height];
    for (int i = 0; i < height; ++i) {
        map[i] = new int[width];
        for (int j = 0; j < width; ++j) {
            file >> map[i][j];
        }
    }

    for (int i = 0; i < 4; ++i) {
        tiles.push_back(IMG_LoadTexture(renderer, ("tile" + std::to_string(i) + ".png").c_str()));
    }

    file.close();
}

Map::~Map() {
    for (int i = 0; i < height; ++i) {
        delete[] map[i];
    }
    delete[] map;
    for (auto tile : tiles) {
        SDL_DestroyTexture(tile);
    }
}

void Map::render(SDL_Renderer* renderer) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            SDL_Rect destRect = {j * 32, i * 32, 32, 32}; // Assume each tile is 32x32 pixels
            SDL_RenderCopy(renderer, tiles[map[i][j]], nullptr, &destRect);
        }
    }
}
