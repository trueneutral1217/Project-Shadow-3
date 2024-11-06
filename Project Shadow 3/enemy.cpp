#include "Enemy.h"
#include "texture.h"  // Adjust this to your texture manager header
#include <cstdlib>
#include <ctime>

Enemy::Enemy(const char* texturePath, SDL_Renderer* renderer, int x, int y)
    : xPos(x), yPos(y), xVel(0.0f), yVel(0.0f), collisionBox(x, y, 16, 16) {  // Initialize collision box
    texture = IMG_LoadTexture(renderer, texturePath);  // Load texture
    std::srand(std::time(0));  // Seed for random number generation

    walkTimer = 0.0f;
    walkDuration = std::rand() % 2 + 1;  // Random duration between 1 and 2 seconds

    // Initialize srcRect and destRect
    srcRect = { 0, 0, 16, 16 };  // Assuming your texture is 16x16 pixels
    destRect = { xPos, yPos, 16, 16 };  // Destination rectangle same size as texture
}

Enemy::~Enemy() {
    SDL_DestroyTexture(texture);
}

void Enemy::update(float deltaTime) {
    walkTimer += deltaTime;
    if (walkTimer >= walkDuration) {
        randomWalk();
        walkTimer = 0.0f;
        walkDuration = std::rand() % 2 + 1;  // Reset walk duration
    }
    collisionBox.updatePosition(xPos, yPos);  // Update collision box position

    // Update destRect position
    destRect.x = xPos;
    destRect.y = yPos;
}

void Enemy::render(SDL_Renderer* renderer, const SDL_Rect& cameraRect) {
    SDL_Rect renderQuad = { xPos - cameraRect.x, yPos - cameraRect.y, destRect.w, destRect.h };
    SDL_RenderCopy(renderer, texture, &srcRect, &renderQuad);
}

void Enemy::randomWalk() {
    int direction = std::rand() % 4;  // Random direction (0: up, 1: down, 2: left, 3: right)
    int moveDistance = 5;  // Distance to move in each step
    switch (direction) {
        case 0: yPos -= moveDistance; break;  // Move up
        case 1: yPos += moveDistance; break;  // Move down
        case 2: xPos -= moveDistance; break;  // Move left
        case 3: xPos += moveDistance; break;  // Move right
    }
}

const CollisionBox& Enemy::getCollisionBox() const {
    return collisionBox;
}
