#include "CollisionBox.h"

CollisionBox::CollisionBox(int x, int y, int w, int h) {
    rect.x = x;
    rect.y = y+(h/2);
    rect.w = w;
    rect.h = h/2;
}

bool CollisionBox::intersects(const CollisionBox& other) const {
    return SDL_HasIntersection(&rect, &other.rect);
}

const SDL_Rect& CollisionBox::getRect() const {
    return rect;
}
