#include "button.h"
#include "texture.h"
#include <iostream>


Button::Button(const std::string& idNormal, const std::string& idHover, SDL_Renderer* renderer, int x, int y, int width, int height) {
        TextureManager& textureManager = TextureManager::getInstance();
        textureManager.loadTexture(idNormal, idNormal, renderer);
        textureManager.loadTexture(idHover, idHover, renderer);

        this->idNormal = idNormal;
        this->idHover = idHover;
        this->rect = { x, y, width, height };
        this->hover = false;
        this->clicked = false;
    }

void Button::update(int mouseX, int mouseY, bool mouseClicked) {
    hover = (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h);
    if (hover && mouseClicked) {
        // Handle button click
        clicked = true;
        std::cout << "\n Button clicked!" << std::endl;
    }
}

void Button::render(SDL_Renderer* renderer) {
        TextureManager& textureManager = TextureManager::getInstance();
        if (hover) {
            textureManager.renderTexture(idHover, renderer, rect.x, rect.y, rect.w, rect.h);
        } else {
            textureManager.renderTexture(idNormal, renderer, rect.x, rect.y, rect.w, rect.h);
        }
}
