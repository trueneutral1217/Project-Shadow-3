#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "SDL_image.h"
#include "Inventory.h"
#include <iostream>
#include "CollisionBox.h"
#include <vector>
#include "InventoryItem.h"

class Player {
    friend class GameState;

public:
    Player(const char* texturePath, SDL_Renderer* renderer, int x, int y);
    ~Player();
    void handleEvents(const SDL_Event& e);
    void update(float deltaTime, const SDL_Rect& cameraRect);
    void render(SDL_Renderer* renderer);
    /*
    void addItemToInventory(const std::string& item);
    void removeItemFromInventory(const std::string& item);
    void showPlayerInventory() const;*/

    void addItem(const InventoryItem& item);
    void equipItem(const std::string& itemName);
    void unequipItem();
    void useItem(const std::string& itemName);
    void updateItemsState();
    bool addContainedItem(const std::string& containerName, const InventoryItem& item);
    bool removeContainedItem(const std::string& containerName, const std::string& itemName);
    const std::vector<InventoryItem>& getInventory() const;

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
    void setHealth(int);
private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    //int xPos, yPos;
    //float xVel, yVel;

    CollisionBox collisionBox;
    CollisionBox interactionBox;
    int health;
    std::vector<InventoryItem> inventory; // List of items in the player's inventory
    InventoryItem* equippedItem; // Pointer to the currently equipped item

};

#endif // PLAYER_H
