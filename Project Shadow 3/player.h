#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "SDL_image.h"
#include "Inventory.h"
#include <iostream>
#include "CollisionBox.h"

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
    float xVel, yVel;
    int xPos, yPos;
    bool playerDead;
    const CollisionBox& getCollisionBox() const {
        return collisionBox;
    }
private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    //int xPos, yPos;
    //float xVel, yVel;
    Inventory inventory;
    CollisionBox collisionBox;
};

#endif // PLAYER_H
