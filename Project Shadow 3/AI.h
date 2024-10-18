#ifndef AI_H
#define AI_H

#include <SDL.h>

class AI {
public:
    AI();
    void update(float deltaTime);
    void setTarget(int x, int y);
    void render(SDL_Renderer* renderer);

private:
    int xPos, yPos;
    int targetX, targetY;
    float speed;
};

#endif // AI_H
