#include "Animation.h"
#include <iostream>

Animation::Animation(int frameWidth, int frameHeight, int frameCount, int frameTime)
    : frameWidth(frameWidth), frameHeight(frameHeight), frameCount(frameCount), frameTime(frameTime), currentFrame(0), elapsedTime(0.0f) {}

void Animation::addFrame(SDL_Texture* texture) {
    frames.push_back(texture);
}

void Animation::update(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime) {
        currentFrame = (currentFrame + 1) % frameCount;
        std::cout<<"\n currentFrame = "<<currentFrame;
        elapsedTime = 0.0f;
    }
}

void Animation::render(SDL_Renderer* renderer, int x, int y) {
    SDL_Rect srcRect = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
    SDL_Rect destRect = { x, y, frameWidth, frameHeight };
    SDL_RenderCopy(renderer, frames[currentFrame], &srcRect, &destRect);
}

void Animation::render(SDL_Renderer* renderer) {
    SDL_Rect srcRect = { 0, 0, frameWidth, frameHeight };
    SDL_Rect destRect = { 0, 0, frameWidth, frameHeight };
    SDL_RenderCopy(renderer, frames[currentFrame], &srcRect, &destRect);
}
