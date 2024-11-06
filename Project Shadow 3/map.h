#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <vector>

class Map {
public:
    Map(const char* mapFile, SDL_Renderer* renderer);
    ~Map();
    void render(SDL_Renderer* renderer);


private:
    std::vector<SDL_Texture*> tiles;
    int** map;
    int width, height;
};

#endif // MAP_H
