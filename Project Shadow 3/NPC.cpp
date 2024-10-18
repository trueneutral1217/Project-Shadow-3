#include "NPC.h"
#include <SDL_image.h>

NPC::NPC(const char* texturePath, SDL_Renderer* renderer, int x, int y, const std::string& dialogue)
    : xPos(x), yPos(y), dialogue(dialogue) {
    texture = IMG_LoadTexture(renderer, texturePath);
    srcRect = {0, 0, 64, 64}; // Assume the NPC sprite is 64x64 pixels
    destRect = {xPos, yPos, 64, 64};
}

NPC::~NPC() {
    SDL_DestroyTexture(texture);
}

void NPC::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

const std::string& NPC::getDialogue() const {
    return dialogue;
}
