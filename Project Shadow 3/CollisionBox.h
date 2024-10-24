#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include <SDL.h>

class CollisionBox {
public:
    CollisionBox(int x, int y, int w, int h);

    bool intersects(const CollisionBox& other) const;
    const SDL_Rect& getRect() const;

private:
    SDL_Rect rect;
};

#endif // COLLISIONBOX_H
