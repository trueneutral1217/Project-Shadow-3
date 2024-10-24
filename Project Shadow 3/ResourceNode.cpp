#include "ResourceNode.h"
#include "texture.h"

ResourceNode::ResourceNode(const std::string& textureId, SDL_Renderer* renderer, int x, int y, int width, int height)
    : textureId(textureId), collisionBox(x, y, width, height) {
    TextureManager& textureManager = TextureManager::getInstance();
    textureManager.loadTexture(textureId, textureId, renderer);
    texture = textureManager.textures[textureId];
}

void ResourceNode::render(SDL_Renderer* renderer) {
    TextureManager::getInstance().renderTexture(textureId, renderer, collisionBox.getRect().x, collisionBox.getRect().y, collisionBox.getRect().w, collisionBox.getRect().h);
}

const CollisionBox& ResourceNode::getCollisionBox() const {
    return collisionBox;
}
