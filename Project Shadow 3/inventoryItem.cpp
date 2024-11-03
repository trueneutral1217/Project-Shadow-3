#include "InventoryItem.h"
#include <ctime>

// Constructor for InventoryItem
InventoryItem::InventoryItem(const std::string& name, const std::string& description, ItemType type, bool equippable, bool singleUse, int maxUses, bool canContainLiquids, bool canContainSolids, int volume, int maxVolume, ItemState initialState)
    : name(name), description(description), type(type), equippable(equippable), singleUse(singleUse), maxUses(maxUses), currentUses(maxUses == -1 ? -1 : maxUses), liquidContainer(canContainLiquids), solidContainer(canContainSolids), volume(volume), maxVolume(maxVolume), currentVolume(0), state(initialState), creationTime(std::time(nullptr)) {}

// Getter for item name
const std::string& InventoryItem::getName() const {
    return name;
}

// Getter for item description
const std::string& InventoryItem::getDescription() const {
    return description;
}

// Getter for item type
InventoryItem::ItemType InventoryItem::getType() const {
    return type;
}

// Check if item is equippable
bool InventoryItem::isEquippable() const {
    return equippable;
}

// Check if item is single-use
bool InventoryItem::isSingleUse() const {
    return singleUse;
}

// Getter for remaining uses
int InventoryItem::getUsesRemaining() const {
    return currentUses;
}

// Check if item can contain liquids
bool InventoryItem::canContainLiquids() const {
    return liquidContainer;
}

// Check if item can contain solids
bool InventoryItem::canContainSolids() const {
    return solidContainer;
}

// Getter for item volume
int InventoryItem::getVolume() const {
    return volume;
}

// Getter for maximum volume the container can hold
int InventoryItem::getMaxVolume() const {
    return maxVolume;
}

// Getter for current volume of items within the container
int InventoryItem::getCurrentVolume() const {
    return currentVolume;
}

// Getter for item state
InventoryItem::ItemState InventoryItem::getState() const {
    return state;
}

// Update the item state based on time elapsed
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
    // Check if the item can be added to the container
    if (((item.canContainLiquids() && liquidContainer) || (item.canContainSolids() && solidContainer)) && (currentVolume + item.getVolume() <= maxVolume)) {
        containedItems.push_back(item); // Add the item to the list of contained items
        currentVolume += item.getVolume(); // Increase the current volume of the container
        return true; // Indicate the item was successfully added
    }
    return false; // Indicate the item could not be added
}

bool InventoryItem::removeContainedItem(const std::string& itemName) {
    // Iterate through the contained items
    for (auto it = containedItems.begin(); it != containedItems.end(); ++it) {
        if (it->getName() == itemName) {
            currentVolume -= it->getVolume(); // Decrease the current volume of the container
            containedItems.erase(it); // Remove the item from the list of contained items
            return true; // Indicate the item was successfully removed
        }
    }
    return false; // Indicate the item could not be found and removed
}

const std::vector<InventoryItem>& InventoryItem::getContainedItems() const {
    return containedItems; // Return the list of contained items
}

void InventoryItem::useItem() {
    if (currentUses > 0 || currentUses == -1) { // Check if the item has uses remaining or has unlimited uses
        if (currentUses > 0) {
            --currentUses; // Decrease the current uses of the item
        }
    }
}

