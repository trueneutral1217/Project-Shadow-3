#include "Player.h"
#include <iostream>

Player::Player(const char* texturePath, SDL_Renderer* renderer, int x, int y)
    : xPos(x), yPos(y), xVel(0), yVel(0) {
    texture = IMG_LoadTexture(renderer, texturePath);
    srcRect = {0, 0, 64, 64}; // Assume the player sprite is 64x64 pixels
    destRect = {xPos, yPos, 64, 64};
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::handleEvents(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP: yVel = -200; break;
            case SDLK_DOWN: yVel = 200; break;
            case SDLK_LEFT: xVel = -200; break;
            case SDLK_RIGHT: xVel = 200; break;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_UP: case SDLK_DOWN: yVel = 0; break;
            case SDLK_LEFT: case SDLK_RIGHT: xVel = 0; break;
        }
    }
}

void Player::update(float deltaTime) {
    xPos += xVel * deltaTime;
    yPos += yVel * deltaTime;
    destRect.x = xPos;
    destRect.y = yPos;
}

void Player::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void Player::addItemToInventory(const std::string& item) {
    inventory.addItem(item);
}

void Player::removeItemFromInventory(const std::string& item) {
    inventory.removeItem(item);
}

void Player::showPlayerInventory() const {
    inventory.showInventory();
}
