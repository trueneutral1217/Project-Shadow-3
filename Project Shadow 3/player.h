#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "SDL_image.h"
#include "Inventory.h"
#include <iostream>
#include "CollisionBox.h"
#include <vector>
#include "InventoryItem.h"
#include "DroppedItem.h"

class Player {
    friend class GameState;

public:
    Player(const char* texturePath, SDL_Renderer* renderer, int x, int y);
    ~Player();
    void handleEvents(const SDL_Event& e);
    void update(float deltaTime, const SDL_Rect& cameraRect);
    void render(SDL_Renderer* renderer);

    //void addItemToInventory(const std::string& item);
    void removeItemFromInventory();
    void showPlayerInventory() const;

    /*
    void setItemX(int newX);
    */
    void dropItem(int itemIndex);
    //void pickUpItem(std::vector<DroppedItem>& droppedItems);
    void pickUpItem(DroppedItem& item);
    int getItemX(int index);

    void addItem(const InventoryItem& item);
    void removeItem();
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
    int getThirst() const;
    void setThirst(int);
    int getHunger() const;
    void setHunger(int);
    int getInventorySize();
    int getMaxInventorySize();

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    //int xPos, yPos;
    //float xVel, yVel;
    int maxInventorySize;
    int inventorySize;
    CollisionBox collisionBox;
    CollisionBox interactionBox;
    int health,hunger,thirst;
    std::vector<InventoryItem> inventory; // List of items in the player's inventory
    InventoryItem* equippedItem; // Pointer to the currently equipped item
    std::vector<DroppedItem> droppedItems; // Track dropped items

};

#endif // PLAYER_H
