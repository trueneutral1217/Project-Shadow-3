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

enum GAMESTATE { SPLASH, MAIN_MENU, IN_GAME, PLAYER_DEAD };
GAMESTATE gameSTATE;

//Other necessary includes and initializations

void handleEvents(SDL_Event& e,GAMESTATE& gameSTATE, Player& player,bool& quit) {
    //user x'd out the window. possibly alt+F4'd.
    if (e.type == SDL_QUIT) {
        quit = true;
    }
    //probably just temporary, I need a way to easily close the game while in development.
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        quit = true;
    }
    switch(gameSTATE){
    case SPLASH:
        //player clicks to skip splash screen.
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            gameSTATE = MAIN_MENU;
        }
        break;
    case MAIN_MENU:
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_n)
        {
            gameSTATE = IN_GAME;
        }
        break;
    case IN_GAME:
        player.handleEvents(e);
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)
        {
            gameSTATE = PLAYER_DEAD;
        }
        break;
    case PLAYER_DEAD:
        //player clicks to skip player dead screen.
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            gameSTATE = MAIN_MENU;
        }
        break;
    }

     //Handle other events
}

void update(GAMESTATE& gameSTATE, Animation& splash, float& deltaTime, Player& player) {
    std::cout<<"\n gameSTATE = "<<gameSTATE;
    switch (gameSTATE) {
        case SPLASH:
            // Your animation update logic

            splash.update(deltaTime);
            if (splash.animationFinished) {
                gameSTATE = MAIN_MENU;
            }
            break;
        case MAIN_MENU:
             //Logic for main menu

            break;
        case IN_GAME:
             //Logic for the main game
            player.update(deltaTime / 1000.0f); // Convert milliseconds to seconds
            if (player.playerDead) {
                gameSTATE = PLAYER_DEAD;
            }
            break;
        case PLAYER_DEAD:
             //Logic for when player is dead
             //Possibly reset to MAIN_MENU or offer options for retry
            break;
    }
}

void render(SDL_Renderer* renderer,GAMESTATE& gameSTATE,Animation& splash,Player& player) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    switch (gameSTATE) {
        case SPLASH:
             //Your rendering code for animation
             splash.render(renderer);
            break;
        case MAIN_MENU:
             //Render main menu options
             TextureManager::getInstance().renderTexture("ps3", renderer, (SCREEN_WIDTH/2)-50, 10, 100, 20);
            break;
        case IN_GAME:
             //Render main game
             player.render(renderer);
            break;
        case PLAYER_DEAD:
             //Render player death screen
             TextureManager::getInstance().renderTexture("gameover", renderer, (SCREEN_WIDTH/2)-50, 10, 100, 20);
            break;
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    gameSTATE = SPLASH;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }

    if (TTF_Init() == -1) {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }

       // Create window in fullscreen mode
    SDL_Window* window = SDL_CreateWindow("Project Shadow 3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 192, SDL_WINDOW_RESIZABLE);//SDL_WINDOW_FULLSCREEN_DESKTOP);

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
    TextureManager::getInstance().loadTextTexture("gameover", "GAME OVER!!!", {255, 255, 255}, font, renderer);
    TextureManager::getInstance().loadTextTexture("ps3", "Project Shadow 3!", {255, 255, 255}, font, renderer);
    Player player("images/ladyInBlue32x32.png", renderer, 100, 100);
    player.playerDead = false;
    GameState gameState("save.dat");
    gameState.load(player);
    bool quit = false;
    SDL_Event e;
    Uint32 startTick, endTick;// deltaTime;
    float deltaTime;
    startTick = SDL_GetTicks();
    //SDL_SetWindowFullscreen( window, SDL_TRUE );

    // Initialize animation with frame count and frame time
    Animation splash(256, 192, 80, 100); // Frame size: 256x192, 80 frames, 100 ms per frame
    splash.animationFinished = false;  //animation still needs to run.
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
            handleEvents(e,gameSTATE,player,quit);
        }
        update(gameSTATE,splash,deltaTime,player);
        render(renderer,gameSTATE,splash,player);
        endTick = SDL_GetTicks();
        deltaTime = endTick - startTick;

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
