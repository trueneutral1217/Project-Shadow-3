#include "CollisionBox.h"

CollisionBox::CollisionBox(int x, int y, int w, int h) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

bool CollisionBox::intersects(const CollisionBox& other) const {
    return SDL_HasIntersection(&rect, &other.rect);
}

const SDL_Rect& CollisionBox::getRect() const {
    return rect;
}
