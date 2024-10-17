#include "HUD.h"
#include <iostream>

HUD::HUD(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font), playerScore(0), playerHealth(100) {
    createTexture("Score: 0", scoreTexture, scoreRect);
    createTexture("Health: 100", healthTexture, healthRect);
}

HUD::~HUD() {
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(healthTexture);
}

void HUD::update(int score, int health) {
    playerScore = score;
    playerHealth = health;
    createTexture("Score: " + std::to_string(score), scoreTexture, scoreRect);
    createTexture("Health: " + std::to_string(health), healthTexture, healthRect);
}

void HUD::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
    SDL_RenderCopy(renderer, healthTexture, nullptr, &healthRect);
}

void HUD::createTexture(const std::string& text, SDL_Texture*& texture, SDL_Rect& rect) {
    SDL_Surface* tempSurface = TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255});
    if (tempSurface == nullptr) {
        std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    rect = {0, 0, tempSurface->w, tempSurface->h};
}
