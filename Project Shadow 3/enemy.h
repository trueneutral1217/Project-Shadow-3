#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <string>

class Enemy {
public:
    Enemy(const char* texturePath, SDL_Renderer* renderer, int x, int y);
    ~Enemy();
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    int xPos, yPos;
    float xVel, yVel;
};

#endif // ENEMY_H
