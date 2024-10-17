#include "texture.h"
#include <iostream>

TextureManager::TextureManager() {}

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

bool TextureManager::loadTexture(const std::string& id, const std::string& filename, SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = IMG_Load(filename.c_str());
    if (tempSurface == nullptr) {
        std::cout << "Failed to load image: " << filename << " Error: " << IMG_GetError() << std::endl;
        return false;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (texture == nullptr) {
        std::cout << "Failed to create texture: " << filename << " Error: " << SDL_GetError() << std::endl;
        return false;
    }
    textures[id] = texture;
    return true;
}

bool TextureManager::loadTextTexture(const std::string& id, const std::string& text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (tempSurface == nullptr) {
        std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
        return false;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (texture == nullptr) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
        return false;
    }
    textures[id] = texture;
    return true;
}

void TextureManager::renderTexture(const std::string& id, SDL_Renderer* renderer, int x, int y, int width, int height) {
    if (textures.find(id) != textures.end()) {
        SDL_Rect destRect = {x, y, width, height};
        SDL_RenderCopy(renderer, textures[id], nullptr, &destRect);
    }
}

void TextureManager::clearTextures() {
    for (auto& texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}
