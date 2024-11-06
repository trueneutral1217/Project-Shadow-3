#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <string>
#include "CollisionBox.h"  // Include the CollisionBox header

class Enemy {
public:
    Enemy(const char* texturePath, SDL_Renderer* renderer, int x, int y);
    ~Enemy();
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, const SDL_Rect& cameraRect);  // Render independently of the camera
    void randomWalk();
    void dead();
    const CollisionBox& getCollisionBox() const;  // Get the collision box

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    int xPos, yPos;
    float xVel, yVel;
    CollisionBox collisionBox;  // Add a collision box

    float walkTimer;
    int walkDuration;
};

#endif // ENEMY_H
