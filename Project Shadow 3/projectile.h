#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SDL.h>

class Projectile {
public:
    Projectile(const char* texturePath, SDL_Renderer* renderer, int x, int y, int dir);
    ~Projectile();
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    int xPos, yPos;
    int direction; // 1 for right, -1 for left
    float speed;
};

#endif // PROJECTILE_H
