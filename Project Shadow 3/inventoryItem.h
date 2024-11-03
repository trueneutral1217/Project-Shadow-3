#ifndef INVENTORYITEM_H // Include guard to prevent multiple inclusions
#define INVENTORYITEM_H

#include <string> // Include the string library for std::string
#include <vector> // Include the vector library for std::vector
#include <ctime> // Include the ctime library for std::time

class InventoryItem { // Define the InventoryItem class
public:
    enum ItemState { FRESH, STALE, SPOILED }; // Enum to represent item states
    enum ItemType { LIQUID, SOLID, EQUIPPABLE }; // Enum to represent item types

    // Constructor with parameters to initialize an item
    InventoryItem(const std::string& name, const std::string& description, ItemType type, bool equippable, bool singleUse, int maxUses = -1, bool canContainLiquids = false, bool canContainSolids = false, int volume = 0, int maxVolume = 0, ItemState initialState = FRESH);

    // Getters for item properties
    const std::string& getName() const;
    const std::string& getDescription() const;
    ItemType getType() const;
    bool isEquippable() const;
    bool isSingleUse() const;
    int getUsesRemaining() const;
    bool canContainLiquids() const;
    bool canContainSolids() const;
    int getVolume() const;
    int getMaxVolume() const;
    int getCurrentVolume() const;
    ItemState getState() const;

    // Methods to update item state and manage contained items
    void updateState();
    bool addContainedItem(const InventoryItem& item);
    bool removeContainedItem(const std::string& itemName);
    const std::vector<InventoryItem>& getContainedItems() const;

    void useItem(); // Method to use the item

private:
    std::string name; // Item name
    std::string description; // Item description
    ItemType type; // Item type (LIQUID, SOLID, or EQUIPPABLE)
    bool equippable; // Indicates if the item can be equipped
    bool singleUse; // Indicates if the item is single-use
    int maxUses; // Maximum uses of the item, -1 for unlimited uses
    int currentUses; // Current remaining uses of the item
    bool liquidContainer; // Indicates if the item can contain liquids
    bool solidContainer; // Indicates if the item can contain solids
    int volume; // Volume required for the item to fit into a container
    int maxVolume; // Maximum volume that the container can hold
    int currentVolume; // Current volume of items within the container
    std::vector<InventoryItem> containedItems; // List of contained items
    ItemState state; // State of the item (FRESH, STALE, SPOILED)
    std::time_t creationTime; // Time when the item was created

    static const int FRESH_DURATION = 1 * 24 * 60 * 60; // Duration for the item to remain fresh (1 day in seconds)
    static const int STALE_DURATION = 2 * 24 * 60 * 60; // Additional duration for the item to become stale (2 days in seconds)
};

#endif // INVENTORYITEM_H
