

#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include <SDL.h>

class CollisionBox {
public:
    CollisionBox(int x, int y, int width, int height);
    bool intersects(const CollisionBox& other) const;
    void updatePosition(int newX, int newY);
    const SDL_Rect& getRect() const;
private:
    SDL_Rect rect;  // SDL rectangle for collision box
};

#endif // COLLISIONBOX_H
