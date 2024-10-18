#ifndef POWERUP_H
#define POWERUP_H

#include <SDL.h>

class PowerUp {
public:
    PowerUp(const char* texturePath, SDL_Renderer* renderer, int x, int y);
    ~PowerUp();
    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
};

#endif // POWERUP_H
