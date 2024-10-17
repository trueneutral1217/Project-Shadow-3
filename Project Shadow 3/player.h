#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "SDL_image.h"
#include "Inventory.h"

class Player {
    friend class GameState;

public:
    Player(const char* texturePath, SDL_Renderer* renderer, int x, int y);
    ~Player();
    void handleEvents(const SDL_Event& e);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void addItemToInventory(const std::string& item);
    void removeItemFromInventory(const std::string& item);
    void showPlayerInventory() const;

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    int xPos, yPos;
    float xVel, yVel;
    Inventory inventory;
};

#endif // PLAYER_H
