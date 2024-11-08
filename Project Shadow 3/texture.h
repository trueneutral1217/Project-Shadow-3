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
    std::map<std::string, SDL_Texture*> textures;
    void render(const std::string& id,SDL_Renderer* renderer,int x, int y, int width, int height, const SDL_Rect& cameraRect);  // Render independently of the camera
    void setAlpha(const std::string& id, Uint8 alpha);
    SDL_Texture* getTexture(const std::string& id);
private:
    TextureManager();
    SDL_Texture* texture;
};

#endif // TEXTUREMANAGER_H
