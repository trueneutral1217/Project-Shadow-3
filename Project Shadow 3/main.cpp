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
#include "button.h"
#include <vector>
#include <cmath>
#include <cstdlib>

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

enum GAMESTATE { SPLASH, MAIN_MENU, IN_GAME, PLAYER_DEAD, OPTIONS_MENU };
GAMESTATE gameSTATE;

//Other necessary includes and initializations

void handleEvents(SDL_Event& e, GAMESTATE& gameSTATE, Player& player, bool& quit, Button& button1, Button& button2, Button& button3, Button& button4) {
    //user x'd out the window. possibly alt+F4'd.
    if (e.type == SDL_QUIT) {
        quit = true;
    }
    //probably just temporary, I need a way to easily close the game while in development.
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        quit = true;
    }
    int mouseX, mouseY;
    bool mouseClicked = false;
    switch(gameSTATE){
    case SPLASH:
        //player clicks to skip splash screen.
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            gameSTATE = MAIN_MENU;
        }
        break;
    case MAIN_MENU:
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            mouseClicked = true;
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        button1.update(mouseX, mouseY, mouseClicked);
        button2.update(mouseX, mouseY, mouseClicked);
        button3.update(mouseX, mouseY, mouseClicked);
        button4.update(mouseX, mouseY, mouseClicked);
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
    case OPTIONS_MENU:

        break;
    }

     //Handle other events
}

void update(GAMESTATE& gameSTATE, Animation& splash, float& deltaTime, Player& player, Button& button1, Button& button2, Button& button3, Button& button4,bool& quit, int height, int width, std::vector<std::vector<int>>& map) {

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
            if(button1.clicked)
            {
                gameSTATE = IN_GAME;
                button1.clicked = false;
                SoundManager::getInstance().playSound("bonus");
            }
            if(button2.clicked)
            {
                //note that savegame variables should be loaded from file here
                gameSTATE = IN_GAME;
                std::cout<<"\n Generating map...";
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        std::cout << map[y][x] << " ";
                    }
                    std::cout << std::endl;
                }
                button2.clicked = false;
                SoundManager::getInstance().playSound("bonus");
            }
            if(button3.clicked)
            {
                gameSTATE = OPTIONS_MENU;
                button3.clicked = false;
                SoundManager::getInstance().playSound("bonus");
            }
            if(button4.clicked)
            {
                quit = true;
                button4.clicked = false;
                SoundManager::getInstance().playSound("bonus");
            }
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
        case OPTIONS_MENU:

            break;
    }
}

void render(SDL_Renderer* renderer,GAMESTATE& gameSTATE,Animation& splash,Player& player, Button& button1, Button& button2, Button& button3, Button& button4, float& deltaTime, int height, int width, std::vector<std::vector<int>>& map) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    switch (gameSTATE) {
        case SPLASH:
             //Your rendering code for animation
             splash.render(renderer);
            break;
        case MAIN_MENU:
            //Render main menu options
            TextureManager::getInstance().renderTexture("mmbg",renderer,0,0,256,192);
            TextureManager::getInstance().renderTexture("titlebg",renderer,(SCREEN_WIDTH/2)-50,60,120,120);
            TextureManager::getInstance().renderTexture("ps3", renderer, (SCREEN_WIDTH/2)-24, 110, 72, 20);
            // Render buttons
            button1.render(renderer);
            button2.render(renderer);
            button3.render(renderer);
            button4.render(renderer);

            break;
        case IN_GAME:
             //Render main game
                 // Print map for visualization
                 for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        std::cout << map[y][x] << " ";
                        TextureManager::getInstance().renderTexture("groundTile",renderer,x*16,y*16,16,16);
                    }
                    std::cout << std::endl;
                }
            /*
             TextureManager::getInstance().renderTexture("ground",renderer,0,0,256,192);
             TextureManager::getInstance().renderTexture("tree1",renderer,100,100,32,32);
            TextureManager::getInstance().renderTexture("tree2",renderer,(int)deltaTime,(int)deltaTime,32,32);
            TextureManager::getInstance().renderTexture("bush",renderer,200,140,16,16);
            TextureManager::getInstance().renderTexture("rock",renderer,170,60,16,16);*/
             player.render(renderer);
            break;
        case PLAYER_DEAD:
             //Render player death screen
             TextureManager::getInstance().renderTexture("gameover", renderer, (SCREEN_WIDTH/2)-50, 10, 100, 20);
            break;
        case OPTIONS_MENU:

            break;
    }

    SDL_RenderPresent(renderer);
}

// Perlin Noise Function
float perlin(float x, float y) {
    // Implementation of Perlin noise
    // This is a placeholder. Replace it with a proper Perlin noise function
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

// Generate Procedural Map
void generateMap(int width, int height, std::vector<std::vector<int>>& map) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float noise = perlin(x * 0.1, y * 0.1); // Scale noise for larger maps
            if (noise > 0.7) {
                map[y][x] = 1; // Example: 1 for water
            } else if (noise > 0.4) {
                map[y][x] = 1; // Example: 2 for grass
            } else {
                map[y][x] = 1; // Example: 3 for mountains
            }
        }
    }
}

int main(int argc, char* args[]) {
    const int width = 16;
    const int height = 12;
    std::vector<std::vector<int>> map(height, std::vector<int>(width, 0));

    generateMap(width, height, map);
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
    SDL_SetWindowFullscreen( window, SDL_TRUE );
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
  SoundManager::getInstance().loadSound("bonus", "sounds/bonus.wav");
  SoundManager::getInstance().loadMusic("background", "music/build.mp3");

    TTF_Font* font = TTF_OpenFont("fonts/PublicPixel-z84yD.ttf", 8);
    if (font == nullptr) {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }
    TextureManager::getInstance().loadTextTexture("gameover", "GAME OVER!!!", {255, 255, 255}, font, renderer);
    TextureManager::getInstance().loadTextTexture("ps3", "Project Shadow 3", {255, 255, 255}, font, renderer);
    TextureManager::getInstance().loadTexture("mmbg","images/mainMenuBackground.png",renderer);
    TextureManager::getInstance().loadTexture("titlebg","images/titleBG.png",renderer);
    TextureManager::getInstance().loadTexture("ground","images/ground.png",renderer);
    TextureManager::getInstance().loadTexture("bird","images/bird1.png",renderer);
    TextureManager::getInstance().loadTexture("bowl","images/bowl.png",renderer);
    TextureManager::getInstance().loadTexture("bowlSoup","images/bowlSoup.png",renderer);
    TextureManager::getInstance().loadTexture("bunny","images/bunny1.png",renderer);
    TextureManager::getInstance().loadTexture("bush","images/bush1.png",renderer);
    TextureManager::getInstance().loadTexture("cloud","images/cloud1.png",renderer);
    TextureManager::getInstance().loadTexture("cup","images/cup.png",renderer);
    TextureManager::getInstance().loadTexture("cupWater","images/cupWater.png",renderer);
    TextureManager::getInstance().loadTexture("moon","images/moon1.png",renderer);
    TextureManager::getInstance().loadTexture("pot1","images/pot1.png",renderer);
    TextureManager::getInstance().loadTexture("pot2","images/pot2.png",renderer);
    TextureManager::getInstance().loadTexture("pot3","images/pot3.png",renderer);
    TextureManager::getInstance().loadTexture("pot3water","images/pot3water.png",renderer);
    TextureManager::getInstance().loadTexture("purpleTurtle","images/purpleTurtle.png",renderer);
    TextureManager::getInstance().loadTexture("rock","images/rock1.png",renderer);
    TextureManager::getInstance().loadTexture("squirrel","images/squirrel1.png",renderer);
    TextureManager::getInstance().loadTexture("sun","images/sun1.png",renderer);
    TextureManager::getInstance().loadTexture("tree1","images/tree1.png",renderer);
    TextureManager::getInstance().loadTexture("tree2","images/tree2.png",renderer);
    TextureManager::getInstance().loadTexture("walnuts","images/walnuts.png",renderer);
    TextureManager::getInstance().loadTexture("groundTile","images/tiles/groundTile.png",renderer);


    Button button1("images/buttons/new.png", "images/buttons/newMO.png", renderer, 10, 10, 100, 50);
    Button button2("images/buttons/load.png", "images/buttons/loadMO.png", renderer, 10, 70, 100, 50);
    Button button3("images/buttons/options.png", "images/buttons/optionsMO.png", renderer, 10, 130, 100, 50);
    Button button4("images/buttons/exit.png","images/buttons/exitMO.png",renderer,156,14,100,50);

    Player player("images/ladyInBlue16x16.png", renderer, (SCREEN_WIDTH/2)-8,(SCREEN_HEIGHT/2)-8);
    player.playerDead = false;
    GameState gameState("save.dat");
    gameState.load(player);
    bool quit = false;
    SDL_Event e;
    Uint32 startTick, endTick;// deltaTime;
    float deltaTime;
    startTick = SDL_GetTicks();


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

    SoundManager::getInstance().playMusic("background");

    while (!quit) {
        endTick = SDL_GetTicks();

        float deltaTime = static_cast<float>(endTick - startTick);
        //deltaTime = (endTick - startTick)/ 1000.0f;
        startTick = endTick;
        while (SDL_PollEvent(&e) != 0) {
            handleEvents(e,gameSTATE,player,quit,button1,button2,button3,button4);
        }
        update(gameSTATE,splash,deltaTime,player,button1,button2,button3,button4,quit,height,width,map);
        render(renderer,gameSTATE,splash,player,button1,button2,button3,button4,deltaTime,height,width,map);
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
