#include "ResourceNode.h"
#include "texture.h"

ResourceNode::ResourceNode(const std::string& textureId, SDL_Renderer* renderer, int x, int y, int width, int height)
    : textureId(textureId), collisionBox(x, y, width, height), interactionBox(x - 1, y - 1, width + 2, height + 2) {
    xPos = x;
    yPos = y;
    w = width;
    h = height;
    TextureManager& textureManager = TextureManager::getInstance();
    textureManager.loadTexture(textureId, textureId, renderer);
    texture = textureManager.textures[textureId];
}

void ResourceNode::render(SDL_Renderer* renderer) {
    TextureManager::getInstance().renderTexture(textureId, renderer, xPos, yPos, w, h);
}

const CollisionBox& ResourceNode::getCollisionBox() const {
    return collisionBox;
}

const CollisionBox& ResourceNode::getInteractionBox() const {
    return interactionBox;
}

SDL_Texture* ResourceNode::getTexture() const {
    return texture;
}

std::string ResourceNode::getTextureId() const{
    return textureId;
}
