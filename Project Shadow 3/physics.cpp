#include "Physics.h"

void Physics::applyGravity(float& yVelocity, float deltaTime, float gravity) {
    yVelocity += gravity * deltaTime;
}

void Physics::applyFriction(float& xVelocity, float friction) {
    xVelocity *= friction;
}
