#ifndef PHYSICS_H
#define PHYSICS_H

#include <SDL.h>

class Physics {
public:
    static void applyGravity(float& yVelocity, float deltaTime, float gravity = 9.8f);
    static void applyFriction(float& xVelocity, float friction = 0.9f);
};

#endif // PHYSICS_H
