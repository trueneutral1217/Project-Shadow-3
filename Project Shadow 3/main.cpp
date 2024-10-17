#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Player.h"
#include "GameState.h"
#include "SoundManager.h"
#include "texture.h"

const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 192;

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }

    if (TTF_Init() == -1) {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return -1;
    }

    // Load assets
    TextureManager::getInstance().loadTexture("player", "images/ladyInBlue32x32.png", renderer);
  //SoundManager::getInstance().loadSound("jump", "jump.wav");
 // SoundManager::getInstance().loadMusic("background", "background.mp3");

    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (font == nullptr) {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }
    TextureManager::getInstance().loadTextTexture("text", "Welcome to SDL Game!", {255, 255, 255}, font, renderer);

    Player player("player.png", renderer, 100, 100);
    GameState gameState("save.dat");

    bool quit = false;
    SDL_Event e;
    Uint32 startTick, endTick, deltaTime;

    SoundManager::getInstance().playMusic("background");

    while (!quit) {
        startTick = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            player.handleEvents(e);
        }

        deltaTime = SDL_GetTicks() - startTick;

        player.update(deltaTime / 1000.0f); // Convert milliseconds to seconds

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        player.render(renderer);
        TextureManager::getInstance().renderTexture("text", renderer, 50, 50, 300, 50);

        SDL_RenderPresent(renderer);

        endTick = SDL_GetTicks();
        deltaTime = endTick - startTick;

        if (deltaTime < 16) {
            SDL_Delay(16 - deltaTime); // Cap the frame rate to ~60 FPS
        }
    }

    gameState.save(player);

    // Cleanup
    SoundManager::getInstance().stopMusic();
    TextureManager::getInstance().clearTextures();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
