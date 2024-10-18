#include "PowerUp.h"
#include <SDL_image.h>

PowerUp::PowerUp(const char* texturePath, SDL_Renderer* renderer, int x, int y) {
    texture = IMG_LoadTexture(renderer, texturePath);
    srcRect = {0, 0, 32, 32}; // Assume the power-up sprite is 32x32 pixels
    destRect = {x, y, 32, 32};
}

PowerUp::~PowerUp() {
    SDL_DestroyTexture(texture);
}

void PowerUp::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

SDL_Rect PowerUp::getRect() const {
    return destRect;
}
