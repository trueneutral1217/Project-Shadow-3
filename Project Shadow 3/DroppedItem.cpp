#include "DroppedItem.h"

DroppedItem::DroppedItem(const InventoryItem& item, int x, int y,SDL_Texture* texture)
    : item(item),x(x),y(y), collisionBox({x, y, 16, 16}),texture(texture) {}

void DroppedItem::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, item.getTexture(), NULL, &collisionBox);
}

const SDL_Rect& DroppedItem::getCollisionBox() const {
    return collisionBox;
}

const InventoryItem& DroppedItem::getItem() const {
    return item;
}
