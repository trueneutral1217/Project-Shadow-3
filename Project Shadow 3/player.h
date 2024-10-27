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
    void update(float deltaTime, const SDL_Rect& cameraRect);
    void render(SDL_Renderer* renderer);
    void addItemToInventory(const std::string& item);
    void removeItemFromInventory(const std::string& item);
    void showPlayerInventory() const;
    float xVel, yVel;
    int xPos, yPos;
    void setPosition(int x, int y);
    void getPosition(int& x, int& y) const;
    const CollisionBox& getCollisionBox() const;
    const CollisionBox& getInteractionBox() const;
    SDL_Texture* getTexture() const;
    void decreaseHealth(int amount);
    void increaseHealth(int amount);
    int getHealth() const;
private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    //int xPos, yPos;
    //float xVel, yVel;
    Inventory inventory;
    CollisionBox collisionBox;
    CollisionBox interactionBox;
    int health;
};

#endif // PLAYER_H
