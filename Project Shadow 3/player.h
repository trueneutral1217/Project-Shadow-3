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

//add melee skill when player defeats an enemy.

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

    /*
    void unhotBarItem1();
    void unhotBarItem2();
    void unhotBarItem3();
    void unhotBarItem4();
    void unhotBarItem5();
    void unhotBarItem6();
    void unhotBarItem7();
    void unhotBarItem8();*/



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
    void decreaseThirst(int amount);
    void increaseThirst(int amount);
    int getHunger() const;
    void setHunger(int);
    void decreaseHunger(int amount);//note, decrease not good for player.
    void increaseHunger(int amount);//player ate something
    int getInventorySize();
    int getMaxInventorySize();

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    float hungerAccumulation;
    //int xPos, yPos;
    //float xVel, yVel;
    int maxInventorySize;
    int inventorySize;
    CollisionBox collisionBox;
    CollisionBox interactionBox;
    int health,hunger,thirst;
    std::vector<InventoryItem> inventory; // List of items in the player's inventory
    /*
    bool hotBar1empty,hotBar2empty,hotBar3empty,hotBar4empty,hotBar5empty,hotBar6empty,hotBar7empty,hotBar8empty;
    InventoryItem* hotBarItem1;
    InventoryItem* hotBarItem2;
    InventoryItem* hotBarItem3;
    InventoryItem* hotBarItem4;
    InventoryItem* hotBarItem5;
    InventoryItem* hotBarItem6;
    InventoryItem* hotBarItem7;
    InventoryItem* hotBarItem8;*/
    InventoryItem* equippedItem; // Pointer to the currently equipped item
    std::vector<DroppedItem> droppedItems; // Track dropped items

};

#endif // PLAYER_H
