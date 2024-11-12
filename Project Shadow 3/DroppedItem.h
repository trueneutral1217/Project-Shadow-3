#ifndef DROPPEDITEM_H
#define DROPPEDITEM_H

#include <SDL.h>
#include "InventoryItem.h"
#include <cstdlib>
#include <algorithm>

class DroppedItem {
public:
    DroppedItem(const InventoryItem& item, int x, int y,SDL_Texture* texture);
    void render(SDL_Renderer* renderer);
    const SDL_Rect& getCollisionBox() const;
    const InventoryItem& getItem() const; // Add this method to get the InventoryItem
    int getX() const { return x;}
    int getY() const { return y; }

private:
    InventoryItem item;
    int x, y;
    SDL_Rect collisionBox;
    SDL_Texture* texture;
};

#endif // DROPPEDITEM_H
