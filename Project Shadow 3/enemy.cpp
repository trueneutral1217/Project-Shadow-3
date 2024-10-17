#include "Enemy.h"
#include <SDL2/SDL_image.h>

Enemy::Enemy(const char* texturePath, SDL_Renderer* renderer, int x, int y)
    : xPos(x), yPos(y), xVel(0), yVel(0) {
    texture = IMG_LoadTexture(renderer, texturePath);
    srcRect = {0, 0, 64, 64}; // Assume the enemy sprite is 64x64 pixels
    destRect = {xPos, yPos, 64, 64};
}

Enemy::~Enemy() {
    SDL_DestroyTexture(texture);
}

void Enemy::update(float deltaTime) {
    xPos += xVel * deltaTime;
    yPos += yVel * deltaTime;
    destRect.x = xPos;
    destRect.y = yPos;
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
