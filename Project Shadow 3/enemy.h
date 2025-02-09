#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <string>
#include "CollisionBox.h"  // Include the CollisionBox header

//Enemies need to drop items when butchered (bones, pelts, meat, sinew)

class Enemy {
public:
    Enemy(const char* texturePath, SDL_Renderer* renderer, int x, int y);
    ~Enemy();
    void update(float deltaTime, int x, int y);
    void render(SDL_Renderer* renderer, const SDL_Rect& cameraRect);  // Render independently of the camera
    void randomWalk();
    bool aggroRange(int x, int y);
    void seekPlayer(int x, int y);
    void avoidPlayer(int x, int y);
    const CollisionBox& getCollisionBox() const;  // Get the collision box
    SDL_Texture* getTexture() const;
    std::string getTextureName();

private:
    std::string texturePathString;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    int xPos, yPos;
    float xVel, yVel;
    CollisionBox collisionBox;  // Add a collision box

    float walkTimer;
    float walkDuration;
};

#endif // ENEMY_H
