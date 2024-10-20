#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <string>

class Button {
public:
    Button(const std::string& idNormal, const std::string& idHover, SDL_Renderer* renderer, int x, int y, int width, int height);

    void update(int mouseX, int mouseY, bool mouseClicked);
    void render(SDL_Renderer* renderer);
    bool clicked;

private:
    std::string idNormal;
    std::string idHover;
    SDL_Rect rect;
    bool hover;
};

#endif // BUTTON_H
