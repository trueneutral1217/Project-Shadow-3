
#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

class Camera {
public:
    Camera(int screenWidth, int screenHeight, int mapWidth, int mapHeight);

    void update(int playerX, int playerY);
    SDL_Rect getCameraRect() const;

private:
    SDL_Rect cameraRect;
    int screenWidth;
    int screenHeight;
    int mapWidth;
    int mapHeight;
};

#endif // CAMERA_H
