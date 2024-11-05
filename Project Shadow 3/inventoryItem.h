#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include <string>
#include <vector>
#include <SDL.h> // Include SDL for rendering

class InventoryItem {
public:
    enum ItemState { FRESH, STALE, SPOILED };
    enum ItemType { LIQUID, SOLID, EQUIPPABLE };

    InventoryItem(SDL_Renderer* renderer,const std::string& name, const std::string& description,const std::string& iconTextureId, ItemType type, bool equippable, bool singleUse, int maxUses = -1, bool canContainLiquids = false, bool canContainSolids = false, int volume = 0, int maxVolume = 0, ItemState initialState = FRESH);

    const std::string& getName() const;
    const std::string& getDescription() const;
    const std::string& getIconTextureId() const;
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
    void updateState();
    bool addContainedItem(const InventoryItem& item);
    bool removeContainedItem(const std::string& itemName);
    const std::vector<InventoryItem>& getContainedItems() const;
    void renderIcon(SDL_Renderer* renderer, int x, int y) const; // New method to render the icon
    void useItem();
    SDL_Texture* getTexture() const;

private:
    std::string name;
    std::string description;
    ItemType type;
    bool equippable;
    bool singleUse;
    int maxUses;
    int currentUses;
    bool liquidContainer;
    bool solidContainer;
    int volume;
    int maxVolume;
    int currentVolume;
    std::vector<InventoryItem> containedItems;
    ItemState state;
    time_t creationTime;
    std::string iconTextureId; // Texture ID for the item's icon
    SDL_Texture* texture;
    static const int FRESH_DURATION = 1 * 24 * 60 * 60; // 1 day in seconds
    static const int STALE_DURATION = 2 * 24 * 60 * 60; // Additional 2 days in seconds
};

#endif // INVENTORYITEM_H
