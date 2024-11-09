#ifndef RESOURCENODE_H
#define RESOURCENODE_H

#include <SDL.h>
#include <string>
#include "texture.h"
#include "CollisionBox.h"

class ResourceNode {
public:
    ResourceNode(const std::string& textureId, SDL_Renderer* renderer, int x, int y, int width, int height);

    void render(SDL_Renderer* renderer);
    const CollisionBox& getCollisionBox() const;
    const CollisionBox& getInteractionBox() const;
    SDL_Texture* getTexture() const;
    std::string getTextureId() const;

private:
    CollisionBox interactionBox;
    std::string textureId;
    SDL_Texture* texture;
    CollisionBox collisionBox;
    int xPos;
    int yPos;
    int w;
    int h;
};

#endif // RESOURCENODE_H
