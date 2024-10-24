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

private:
    std::string textureId;
    SDL_Texture* texture;
    CollisionBox collisionBox;
};

#endif // RESOURCENODE_H
