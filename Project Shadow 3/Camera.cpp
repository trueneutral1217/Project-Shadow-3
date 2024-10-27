
#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight, int mapWidth, int mapHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight), mapWidth(mapWidth), mapHeight(mapHeight) {
    cameraRect = {0, 0, screenWidth, screenHeight};
}

void Camera::update(int playerX, int playerY) {
    cameraRect.x = playerX - screenWidth / 2;
    cameraRect.y = playerY - screenHeight / 2;

    if (cameraRect.x < 0) {
        cameraRect.x = 0;
    }
    if (cameraRect.y < 0) {
        cameraRect.y = 0;
    }
    if (cameraRect.x > mapWidth - screenWidth) {
        cameraRect.x = mapWidth - screenWidth;
    }
    if (cameraRect.y > mapHeight - screenHeight) {
        cameraRect.y = mapHeight - screenHeight;
    }
}

SDL_Rect Camera::getCameraRect() const {
    return cameraRect;
}
