#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL_image.h"
#include <string>
#include <map>

class TextureManager {
public:
    static TextureManager& getInstance();
    bool loadTexture(const std::string& id, const std::string& filename, SDL_Renderer* renderer);
    bool loadTextTexture(const std::string& id, const std::string& text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer);
    void renderTexture(const std::string& id, SDL_Renderer* renderer, int x, int y, int width, int height);
    void clearTextures();

private:
    TextureManager();
    std::map<std::string, SDL_Texture*> textures;
};

#endif // TEXTUREMANAGER_H
