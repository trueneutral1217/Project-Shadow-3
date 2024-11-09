#include "InventoryItem.h"
#include "Texture.h" // Include the texture manager
#include <ctime>
#include <iostream>

InventoryItem::InventoryItem(SDL_Renderer* renderer,const std::string& name, const std::string& description,const std::string& iconTextureId, ItemType type, bool equippable, bool singleUse, int maxUses, bool canContainLiquids, bool canContainSolids, int volume, int maxVolume, ItemState initialState)
    : name(name), description(description),iconTextureId(iconTextureId), type(type), equippable(equippable), singleUse(singleUse), maxUses(maxUses), currentUses(maxUses == -1 ? -1 : maxUses), liquidContainer(canContainLiquids), solidContainer(canContainSolids), volume(volume), maxVolume(maxVolume), currentVolume(0), state(initialState), creationTime(std::time(nullptr))
    {
        TextureManager& textureManager = TextureManager::getInstance();
        textureManager.loadTexture(iconTextureId, iconTextureId, renderer);
        texture = textureManager.textures[iconTextureId];
        y=174;

    }

const std::string& InventoryItem::getName() const {
    return name;
}

const std::string& InventoryItem::getDescription() const {
    return description;
}

const std::string& InventoryItem::getIconTextureId() const{
    return iconTextureId;
}

InventoryItem::ItemType InventoryItem::getType() const {
    return type;
}

bool InventoryItem::isEquippable() const {
    return equippable;
}

bool InventoryItem::isSingleUse() const {
    return singleUse;
}

int InventoryItem::getUsesRemaining() const {
    return currentUses;
}

bool InventoryItem::canContainLiquids() const {
    return liquidContainer;
}

bool InventoryItem::canContainSolids() const {
    return solidContainer;
}

int InventoryItem::getVolume() const {
    return volume;
}

int InventoryItem::getMaxVolume() const {
    return maxVolume;
}

int InventoryItem::getCurrentVolume() const {
    return currentVolume;
}

InventoryItem::ItemState InventoryItem::getState() const {
    return state;
}

void InventoryItem::updateState() {
    std::time_t currentTime = std::time(nullptr);
    double elapsedTime = std::difftime(currentTime, creationTime);

    if (elapsedTime > FRESH_DURATION + STALE_DURATION) {
        state = SPOILED;
    } else if (elapsedTime > FRESH_DURATION) {
        state = STALE;
    }
}

bool InventoryItem::addContainedItem(const InventoryItem& item) {
    if (((item.canContainLiquids() && liquidContainer) || (item.canContainSolids() && solidContainer)) && (currentVolume + item.getVolume() <= maxVolume)) {
        containedItems.push_back(item);
        currentVolume += item.getVolume();
        return true;
    }
    return false;
}

bool InventoryItem::removeContainedItem(const std::string& itemName) {
    for (auto it = containedItems.begin(); it != containedItems.end(); ++it) {
        if (it->getName() == itemName) {
            currentVolume -= it->getVolume();
            containedItems.erase(it);
            return true;
        }
    }
    return false;
}

const std::vector<InventoryItem>& InventoryItem::getContainedItems() const {
    return containedItems;
}

void InventoryItem::renderIcon(SDL_Renderer* renderer, int x, int y) const {
    TextureManager::getInstance().renderTexture(iconTextureId, renderer, x, y, 16, 16); // Render the icon at the specified position
}

void InventoryItem::useItem() {
    std::cout<<"\n currentUses: "<<currentUses;
    if (currentUses > 0 || currentUses == -1) {
        if (currentUses > 0) {
            currentUses--;
            if(currentUses == 0)
            {
                name = "";
                SDL_Texture* tempTexture;
                texture = tempTexture;
            }
        }
    }

}

SDL_Texture* InventoryItem::getTexture() const {
    return texture;
}

void InventoryItem::removeItem() {

}

void InventoryItem::setX(int newX)
{
    x = newX;
}

void InventoryItem::setY(int newY)
{
    y = newY;
}
int InventoryItem::getX()
{
    return x;
}
int InventoryItem::getY()
{
    return y;
}
