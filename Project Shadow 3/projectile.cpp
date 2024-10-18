#include "Projectile.h"
#include <SDL_image.h>

Projectile::Projectile(const char* texturePath, SDL_Renderer* renderer, int x, int y, int dir)
    : xPos(x), yPos(y), direction(dir), speed(300.0f) {
    texture = IMG_LoadTexture(renderer, texturePath);
    srcRect = {0, 0, 32, 32}; // Assume the projectile sprite is 32x32 pixels
    destRect = {xPos, yPos, 32, 32};
}

Projectile::~Projectile() {
    SDL_DestroyTexture(texture);
}

void Projectile::update(float deltaTime) {
    xPos += direction * speed * deltaTime;
    destRect.x = xPos;
}

void Projectile::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
