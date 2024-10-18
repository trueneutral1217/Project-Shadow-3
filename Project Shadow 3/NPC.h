#ifndef NPC_H
#define NPC_H

#include <SDL.h>
#include <string>

class NPC {
public:
    NPC(const char* texturePath, SDL_Renderer* renderer, int x, int y, const std::string& dialogue);
    ~NPC();
    void render(SDL_Renderer* renderer);
    const std::string& getDialogue() const;

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    int xPos, yPos;
    std::string dialogue;
};

#endif // NPC_H
