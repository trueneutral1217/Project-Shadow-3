#ifndef HUD_H
#define HUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class HUD {
public:
    HUD(SDL_Renderer* renderer, TTF_Font* font);
    ~HUD();
    void update(int score, int health);
    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* scoreTexture;
    SDL_Texture* healthTexture;
    SDL_Rect scoreRect;
    SDL_Rect healthRect;
    int playerScore;
    int playerHealth;
    SDL_Renderer* renderer;
    TTF_Font* font;
    void createTexture(const std::string& text, SDL_Texture*& texture, SDL_Rect& rect);
};

#endif // HUD_H
