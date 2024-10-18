#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "player.h"
#include "gameState.h"
#include "soundManager.h"
#include "texture.h"
#include <string>
#include <iostream>
#include "animation.h"

//note, eventManager (eventManager seemed incomplete, ask Phaedra) and network have problems (network needs proper SDL libraries in place and linked).
//particleSystem.cpp has a reference to std::remove_if and needs the library added.
//most of the errors were being thrown as std::cerr, I've changed them all to std::cout, though I may load the library and put them
//into play again.
/*
#include "AI.h"

#include "collision.h"
#include "eventManager.h"
#include "HUD.h"
#include "map.h"
#include "inventory.h"
#include "inventoryItem.h"
#include "network.h"
#include "NPC.h"
#include "particleSystem.h"
#include "physics.h"
#include "powerUp.h"
#include "projectile.h"
#include "questSystem.h"
#include "settings.h"
*/


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

       // Create window in fullscreen mode
    SDL_Window* window = SDL_CreateWindow("Project Shadow 3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 192, SDL_WINDOW_FULLSCREEN_DESKTOP);

    //SDL_Window* window = SDL_CreateWindow("Project Shadow 3", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return -1;
    }
    // Set logical size for the viewport
    SDL_RenderSetLogicalSize(renderer, 256, 192);
    // Load assets
    TextureManager::getInstance().loadTexture("player", "images/ladyInBlue32x32.png", renderer);
  //SoundManager::getInstance().loadSound("jump", "jump.wav");
 // SoundManager::getInstance().loadMusic("background", "background.mp3");

    TTF_Font* font = TTF_OpenFont("fonts/PublicPixel-z84yD.ttf", 12);
    if (font == nullptr) {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }

    TextureManager::getInstance().loadTextTexture("ps3", "Project Shadow 3!", {255, 255, 255}, font, renderer);
    Player player("images/ladyInBlue32x32.png", renderer, 100, 100);
    GameState gameState("save.dat");
    bool quit = false;
    SDL_Event e;
    Uint32 startTick, endTick;// deltaTime;
    float deltaTime;
    startTick = SDL_GetTicks();
    //SDL_SetWindowFullscreen( window, SDL_TRUE );

    // Initialize animation with frame count and frame time
    Animation splash(256, 192, 80, 100); // Frame size: 256x192, 80 frames, 100 ms per frame

    // Load frames in reverse order
    for (int i = 80; i > 0; --i) {
    std::string framePath = "images/animations/splashAnimation/splash" + std::to_string(i) + ".png";
    SDL_Surface* tempSurface = IMG_Load(framePath.c_str());
    if (!tempSurface) {
        std::cerr << "\n Failed to load image: " << framePath << " Error: " << IMG_GetError() << std::endl;
        continue;
    }
    SDL_Texture* frameTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!frameTexture) {
        std::cerr << "\n Failed to create texture: " << IMG_GetError() << std::endl;
        continue;
    }
    splash.addFrame(frameTexture);
    }

    //SoundManager::getInstance().playMusic("background");

    while (!quit) {
        endTick = SDL_GetTicks();

        float deltaTime = static_cast<float>(endTick - startTick);
        //deltaTime = (endTick - startTick)/ 1000.0f;
        startTick = endTick;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
            /*
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (gameState == SPLASH) {
                    gameState = MAIN_MENU;
                }
            }*/
            //this should be un-commented out after gamestates are added.
            //player.handleEvents(e);
        }
        splash.update(deltaTime);
        player.update(deltaTime / 1000.0f); // Convert milliseconds to seconds

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        splash.render(renderer);

        //this should be un-commented out after gamestates are added. destination: in_game
        //player.render(renderer);

        //this should be un-commented out after gamestates are added. destination: main_menu
        //TextureManager::getInstance().renderTexture("ps3", renderer, (SCREEN_WIDTH/2)-50, 10, 100, 20);

        SDL_RenderPresent(renderer);

        endTick = SDL_GetTicks();
        deltaTime = endTick - startTick;
       // std::cout << "startTick: " << startTick << " endTick: " << endTick << " deltaTime: " << deltaTime << std::endl;



        if (deltaTime < 15) {
            SDL_Delay(15 - deltaTime); // Cap the frame rate to ~60 FPS
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
