#include "Player.h"
#include <iostream>

Player::Player(const char* texturePath, SDL_Renderer* renderer, int x, int y)
    : xPos(x), yPos(y),health(100), xVel(0), yVel(0), collisionBox(x, y, 16, 16), interactionBox(x - 1, y - 1, 18, 18) {
    texture = IMG_LoadTexture(renderer, texturePath);
    srcRect = {0, 0, 16, 16}; // Assume the player sprite is 64x64 pixels
    destRect = {xPos, yPos, 16, 16};
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::handleEvents(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                if(yVel>-150)
                {
                    yVel=-150;

                } break;
            case SDLK_s:
                if(yVel < 150)
                {
                    yVel = 150;

                }
            break;
            case SDLK_a:
                if(xVel > -150)
                {
                    xVel = -150;

                }
                break;
            case SDLK_d:
                if(xVel < 150)
                {
                    xVel = 150;

                }
                break;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_w: yVel = 0; break;
            case SDLK_s: yVel = 0; break;
            case SDLK_a: xVel = 0; break;
            case SDLK_d: xVel = 0; break;
        }
    }
}

void Player::update(float deltaTime) {

    xPos += xVel * deltaTime;
    yPos += yVel * deltaTime;

    destRect.x = xPos;
    destRect.y = yPos;

    collisionBox = CollisionBox(xPos, yPos, 16, 16); // Update collision box position
    interactionBox = CollisionBox(xPos - 1, yPos - 1, 18, 18);
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

void Player::setPosition(int x, int y) {
    this->xPos = x;
    this->yPos = y;
    collisionBox = CollisionBox(x, y, collisionBox.getRect().w, collisionBox.getRect().h);
}

void Player::getPosition(int& x, int& y) const {
    x = this->xPos;
    y = this->yPos;
}

const CollisionBox& Player::getCollisionBox() const {
        return collisionBox;
}

const CollisionBox& Player::getInteractionBox() const {
    return interactionBox;
}

void Player::decreaseHealth(int amount) {
    health -= amount;
}

void Player::increaseHealth(int amount) {
    health += amount;
}

int Player::getHealth() const {
    return health;
}
