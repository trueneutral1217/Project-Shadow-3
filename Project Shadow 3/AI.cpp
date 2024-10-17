#include "AI.h"
#include <cmath>

AI::AI() : xPos(0), yPos(0), targetX(0), targetY(0), speed(100.0f) {}

void AI::update(float deltaTime) {
    float dx = targetX - xPos;
    float dy = targetY - yPos;
    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance > 0) {
        xPos += (dx / distance) * speed * deltaTime;
        yPos += (dy / distance) * speed * deltaTime;
    }
}

void AI::setTarget(int x, int y) {
    targetX = x;
    targetY = y;
}

void AI::render(SDL_Renderer* renderer) {
    SDL_Rect rect = {xPos, yPos, 50, 50}; // Assume the AI is represented by a 50x50 square
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
