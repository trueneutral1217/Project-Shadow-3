#include "Player.h"
#include <iostream>

Player::Player(const char* texturePath, SDL_Renderer* renderer, int x, int y)
    : xPos(x), yPos(y),thirst(100),hunger(100),health(100), xVel(0), yVel(0), collisionBox(x+4, y+8, 8, 8), interactionBox(x + 2, y + 6, 12, 12) {
    texture = IMG_LoadTexture(renderer, texturePath);
    srcRect = {0, 0, 16, 16}; // Assume the player sprite is 16x16 pixels
    destRect = {x, y, 16, 16};
    inventorySize=0;
    hotBarSize=0;
    maxHotBarSize = 8;
    maxInventorySize=32;
    hungerAccumulation = 0.0f;

    //
    //hotBar1empty = hotBar2empty = hotBar3empty= hotBar4empty= hotBar5empty=hotBar6empty=hotBar7empty=hotBar8empty=false;
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
    }/*
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_RIGHT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            // Check which item is clicked
            for (size_t i = 0; i < inventory.size(); ++i) {
                //SDL_Rect itemRect = {  position of item in inventory/hotbar  };
                if (SDL_PointInRect(&SDL_Point{ mouseX, mouseY }, &itemRect)) {
                    dropItem(i);
                    break;
                }
            }
        }
    }*/
}

void Player::update(float deltaTime, const SDL_Rect& cameraRect) {
    hungerAccumulation+=deltaTime;
    //std::cout<<"\n hungerAccumulation: "<<hungerAccumulation;
    if(hungerAccumulation >= 3)
    {
        decreaseHunger(1);
        hungerAccumulation = 0.0f;
    }
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

    /*
    // Check for nearby dropped items to pick up
    for (auto& item : droppedItems) {
        if (SDL_HasIntersection(collisionBox.getRect(), &SDL_Rect{item.getX(), item.getY(), 16, 16})) {
            pickUpItem(item);
        }
    }

    // Remove picked up items from the vector
    droppedItems.erase(std::remove_if(droppedItems.begin(), droppedItems.end(), [this](const DroppedItem& item) {
        return SDL_HasIntersection(&collisionBox.getRect(), &SDL_Rect{item.getX(), item.getY(), 16, 16});
    }), droppedItems.end());
    */
}

void Player::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

int Player::getInventorySize(){
    return inventorySize;
}

int Player::getHotBarSize(){
    return hotBarSize;
}

int Player::getMaxInventorySize(){
    return maxInventorySize;
}

int Player::getMaxHotBarSize(){
    return maxHotBarSize;
}
/*
void Player::addItemToInventory(const std::string& item) {
    inventory.addItem(item);
}*/

//void Player::removeItemFromInventory(const std::string& item) {
void Player::removeItemFromInventory() {
    //inventory.removeItem(item);
    if(inventorySize > 0)
    {
        inventory.pop_back();
        inventorySize--;
    }
}

void Player::removeItemFromHotBar() {
    if(slots.size() > 0)
    {
        slots.pop_back();
        hotBarSize--;
    }
}

void Player::showPlayerInventory() const {
//    inventory.showInventory();
}

// Method to add an item to the player's inventory
void Player::addItem(const InventoryItem& item) {
    //item.setX(64+(16*inventorySize));
    inventory.push_back(item); // Add the item to the inventory
    inventory.back().setX(64+(15*inventorySize));
    std::cout << "\n Added item: " << item.getName() << std::endl;
    //setX(64+(16*inventorySize));
    inventorySize++;

}

void Player::addHotBarItem(const InventoryItem& item)
{
    slots.push_back(item);
    slots.back().setX(64+(15*hotBarSize));
    std::cout << "\n added HOT item: " << item.getName() <<std::endl;
    hotBarSize++;
}

void Player::dropItem(int itemIndex){
    if (itemIndex >= 0 && itemIndex < inventory.size())
    {
    InventoryItem item = inventory[itemIndex];
    inventory.erase(inventory.begin() + itemIndex);
    // Create a dropped item entity at the player's position
    // You can use item.getName() and item.getTexture() to create the dropped item entity
    }
}
/*
void Player::pickUpItem(std::vector<DroppedItem>& droppedItems)
{
    for (auto it = droppedItems.begin(); it != droppedItems.end(); )
    {
        if (SDL_HasIntersection(&collisionBox.getRect(), &it->getCollisionBox()))
        {
            addItem(it->getItem());
            it = droppedItems.erase(it);
            // Remove the picked-up item
        }
        else {
            ++it;
        }
    }
}*/

int Player::getInventoryItemX(int index){
    int theX;
    theX = inventory.at(index).getX();
    return theX;
}

int Player::getHotBarItemX(int index){
    int theX;
    theX = slots.at(index).getX();
    return theX;
}

// Method to equip an item by name
void Player::equipItem(const std::string& itemName) {
    for (auto& item : inventory) {
        if (item.getName() == itemName && item.isEquippable()) {
            equippedItem = &item; // Set the equipped item
            std::cout << "\n Equipped item: " << itemName;
            return;
        }
    }
    std::cout << "\n Item not found or not equippable: " << itemName;
}

void Player::updateSlot(int slotIndex, InventoryItem newItem) {
    if (slotIndex >= 0 && slotIndex < slots.size()) {
        slots[slotIndex] = newItem;
    }
}

void Player::renderSlots(SDL_Renderer* renderer) {
        // Render hotbar slots and items here

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
    std::cout<<"\n player Health: "<<health;
    /*
    if(inventory.size()==1)
    {
        inventory.clear();
    }*/
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
    for(auto& item : slots){
        item.updateState();
    }
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

const std::vector<InventoryItem>& Player::getHotBar() const {
    return slots;
}

void Player::setPosition(int x, int y) {
    this->xPos = x;
    this->yPos = y;
    collisionBox = CollisionBox(x+4, y+8, 8, 8);
    interactionBox = CollisionBox(x, y+4,14,14);
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
    std::cout<<"\n player health: "<<health;
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

int Player::getThirst() const {
    return thirst;
}

void Player::setThirst(int newThirst) {
    thirst = newThirst;
    std::cout<<"\n player thirst: "<<thirst;
}
void Player::decreaseThirst(int amount) {
    thirst -= amount;
    std::cout<<"\n player thirst: "<<thirst;
}

void Player::increaseThirst(int amount) {
    thirst += amount;
}

int Player::getHunger() const {
    return hunger;
}

void Player::setHunger(int newHunger) {
    hunger = newHunger;
    std::cout<<"\n player hunger: "<<hunger;
}
void Player::decreaseHunger(int amount) {
    if(hunger>0){
        hunger -= amount;
        std::cout<<"\n player hunger: "<<hunger;
    }
    else{
        health -=amount;
        std::cout<<"\n player health: "<<health;
    }


}

void Player::increaseHunger(int amount) {
    hunger += amount;
    std::cout<<"\n player hunger: "<<hunger;
}
