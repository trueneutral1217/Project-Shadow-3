#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include <vector>

class Animation {
public:
    Animation(int frameWidth, int frameHeight, int frameCount, int frameTime);
    void addFrame(SDL_Texture* texture);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, int x, int y);
    void render(SDL_Renderer* renderer);

private:
    std::vector<SDL_Texture*> frames;
    int frameWidth, frameHeight;
    int frameCount;
    int frameTime; // Time per frame in milliseconds
    int currentFrame;
    float elapsedTime;
};

#endif // ANIMATION_H
