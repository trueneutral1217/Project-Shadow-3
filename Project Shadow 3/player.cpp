#include "Player.h"
#include <iostream>

Player::Player(const char* texturePath, SDL_Renderer* renderer, int x, int y)
    : xPos(x), yPos(y),health(100), xVel(0), yVel(0), collisionBox(x+4, y+8, 8, 8), interactionBox(x + 3, y + 7, 10, 10) {
    texture = IMG_LoadTexture(renderer, texturePath);
    srcRect = {0, 0, 16, 16}; // Assume the player sprite is 16x16 pixels
    destRect = {x, y, 16, 16};
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::handleEvents(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                if(yVel>-150)
                {
                    yVel=-150;

                } break;
            case SDLK_s:
                if(yVel < 150)
                {
                    yVel = 150;

                }
            break;
            case SDLK_a:
                if(xVel > -150)
                {
                    xVel = -150;

                }
                break;
            case SDLK_d:
                if(xVel < 150)
                {
                    xVel = 150;

                }
                break;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_w: yVel = 0; break;
            case SDLK_s: yVel = 0; break;
            case SDLK_a: xVel = 0; break;
            case SDLK_d: xVel = 0; break;
        }
    }
}

void Player::update(float deltaTime, const SDL_Rect& cameraRect) {

    xPos += xVel * deltaTime;
    yPos += yVel * deltaTime;

    destRect.x = xPos;
    destRect.y = yPos;
//these four if statements cover wall collisions
    if (xPos < cameraRect.x) {
        xPos = cameraRect.x;
    }
    if (yPos < cameraRect.y) {
        yPos = cameraRect.y;
    }
    if (xPos + 16 > cameraRect.x + cameraRect.w) {
        xPos = cameraRect.x + cameraRect.w - 16;
    }
    if (yPos + 16 > (cameraRect.y + cameraRect.h)) {
        yPos = (cameraRect.y + cameraRect.h - 16);
    }

    collisionBox = CollisionBox(xPos+4, yPos+8, 8, 8); // Update collision box position
    interactionBox = CollisionBox(xPos +3, yPos + 7 , 10, 10);
}

void Player::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
/*
void Player::addItemToInventory(const std::string& item) {
    inventory.addItem(item);
}

void Player::removeItemFromInventory(const std::string& item) {
    inventory.removeItem(item);
}

void Player::showPlayerInventory() const {
    inventory.showInventory();
}*/

// Method to add an item to the player's inventory
void Player::addItem(const InventoryItem& item) {
    inventory.push_back(item); // Add the item to the inventory
    std::cout << "Added item: " << item.getName() << std::endl;
}

// Method to equip an item by name
void Player::equipItem(const std::string& itemName) {
    for (auto& item : inventory) {
        if (item.getName() == itemName && item.isEquippable()) {
            equippedItem = &item; // Set the equipped item
            std::cout << "Equipped item: " << itemName << std::endl;
            return;
        }
    }
    std::cout << "Item not found or not equippable: " << itemName <<std::endl;
}
// Method to unequip the currently equipped item
void Player::unequipItem() {
    if (equippedItem) {
        std::cout << "Unequipped item: " << equippedItem->getName() << std::endl;
        equippedItem = nullptr; // Clear the equipped item
    } else {
        std::cout << "No item is currently equipped" << std::endl;
    }
}

// Method to use an item by name
void Player::useItem(const std::string& itemName) {
    auto it = inventory.begin();
    while (it != inventory.end()) {
        if (it->getName() == itemName) {
            it->useItem(); // Use the item
            std::cout << "Used item: " << itemName << std::endl;
            if ((it->isSingleUse() || it->getUsesRemaining() == 0) && it->getUsesRemaining() != -1) {
                if (equippedItem == &(*it)) {
                    unequipItem(); // Unequip the item if it was equipped
                }
                it = inventory.erase(it); // Remove the item from the inventory
                std::cout << "Removed item: " << itemName << std::endl;
            } else {
                ++it;
            }
            return;
        } else {
            ++it;
        }
    }
    std::cout << "Item not found: " << itemName << std::endl;
}

// Method to update the state of all items in the inventory
void Player::updateItemsState() {
    for (auto& item : inventory) {
        item.updateState(); // Update the state of each item
    }
}

// Method to add a contained item to a container by name
bool Player::addContainedItem(const std::string& containerName, const InventoryItem& item) {
    for (auto& container : inventory) {
        if (container.getName() == containerName) {
            if (container.addContainedItem(item)) {
                std::cout << "Added item " << item.getName() << " to container " << containerName << std::endl;
                return true;
            } else {
                std::cout << "Failed to add item " << item.getName() << " to container " << containerName << std::endl;
                return false;
            }
        }
    }
    std::cout << "Container not found: " << containerName << std::endl;
    return false;
}

// Method to remove a contained item from a container by name
bool Player::removeContainedItem(const std::string& containerName, const std::string& itemName) {
    for (auto& container : inventory) {
        if (container.getName() == containerName) {
            if (container.removeContainedItem(itemName)) {
                std::cout << "Removed item " << itemName << " from container " << containerName << std::endl;
                return true;
            } else {
                std::cout << "Failed to remove item " << itemName << " from container " << containerName << std::endl;
                return false;
            }
        }
    }
    std::cout << "Container not found: " << containerName << std::endl;
    return false;
}

// Method to get the inventory
const std::vector<InventoryItem>& Player::getInventory() const {
    return inventory; // Return the inventory
}

void Player::setPosition(int x, int y) {
    this->xPos = x;
    this->yPos = y;
    collisionBox = CollisionBox(x+4, y+8, 8, 8);
    interactionBox = CollisionBox(x+3, y+7,10,10);
}

void Player::getPosition(int& x, int& y) const {
    x = this->xPos;
    y = this->yPos;
}

const CollisionBox& Player::getCollisionBox() const {
        return collisionBox;
}

const CollisionBox& Player::getInteractionBox() const {
    return interactionBox;
}

SDL_Texture* Player::getTexture() const {
    return texture;
}

void Player::decreaseHealth(int amount) {
    health -= amount;
}

void Player::increaseHealth(int amount) {
    health += amount;
}

int Player::getHealth() const {
    return health;
}

void Player::setHealth(int newHealth) {
    health = newHealth;
    std::cout<<"\n player health: "<<health;
}
