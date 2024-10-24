#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
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
#include <map>
#include "Timer.h"
#include "ResourceNode.h"

//note, eventManager (eventManager seemed incomplete, ask Phaedra) and network have problems (network needs proper SDL libraries in place and linked).

//set up collision boxes for player, trees, and bushes.
//set up newGameCutScene, text textures for premise/backstory, need timer, and fast forward
//on player clicks, add a skip intro button as well.
//save/load map.
//need slider for volume, background image, and back button for options_menu gamestate.

/*
#include "AI.h"


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
            if(gameSTATE == MAIN_MENU){
                quit = true;
            }
            else
            {
                gameSTATE = MAIN_MENU;
            }
    }
    int mouseX, mouseY;
    bool mouseClicked = false;
    switch(gameSTATE){
    case SPLASH:
        //player clicks to skip splash screen.
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            gameSTATE = MAIN_MENU;
            SoundManager::getInstance().playSound("collect5");
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

void copyMap(const std::vector<std::vector<int>>& src, std::map<int, std::map<int, int>>& dest) {
    for (int i = 0; i < src.size(); ++i) {
        for (int j = 0; j < src[i].size(); ++j) {
            dest[i][j] = src[i][j];
        }
    }
}

void update(GAMESTATE& gameSTATE, Animation& splash, float& deltaTime, Player& player, Button& button1, Button& button2, Button& button3, Button& button4,bool& quit, int height, int width, std::vector<std::vector<int>>& map, std::map<int, std::map<int, int>>& tileMap,std::vector<ResourceNode>& resourceNodes,SDL_Renderer* renderer) {

    switch (gameSTATE) {
        case SPLASH:
            // Your animation update logic

            splash.update(deltaTime);
            if (splash.animationFinished) {
                gameSTATE = MAIN_MENU;
                SoundManager::getInstance().playSound("collect5");
            }
            break;
        case MAIN_MENU:
             //Logic for main menu
            if(button1.clicked)
            {
                gameSTATE = IN_GAME;
                std::cout<<"\n Generating map... \n";
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        std::cout << map[y][x] << " ";
                        if(map[y][x] == 2)
                        {
                            //TextureManager::getInstance().renderTexture("tree1",renderer,(x*16)-16,(y*16)-16,32,32);
                            resourceNodes.emplace_back("images/tree1.png", renderer, (x*16)-16,(y*16)-16 , 32, 32);
                        }
                        else if(map[y][x] == 3)
                        {
                            //TextureManager::getInstance().renderTexture("bush",renderer,x*16,y*16,16,16);
                            resourceNodes.emplace_back("images/bush1.png", renderer, x*16,y*16 , 16, 16);
                        }
                    }
                    std::cout << std::endl;
                }

                copyMap(map, tileMap);

                std::cout<<"\n Copying map to tileMap... \n";
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        std::cout << tileMap[y][x] << " ";
                    }
                    std::cout << std::endl;
                }

                button1.clicked = false;
                SoundManager::getInstance().playSound("bonus");
            }
            if(button2.clicked)
            {
                //note that savegame variables should be loaded from file here
                gameSTATE = IN_GAME;
                //map needs to be loaded from save here.
                button2.clicked = false;
                SoundManager::getInstance().playSound("birdchirp");
            }
            if(button3.clicked)
            {
                gameSTATE = OPTIONS_MENU;
                button3.clicked = false;
                SoundManager::getInstance().playSound("compute");
            }
            if(button4.clicked)
            {
                quit = true;
                button4.clicked = false;
                SoundManager::getInstance().playSound("fail");
            }
            break;
        case IN_GAME:
             //Logic for the main game
            player.update(deltaTime / 1000.0f); // Convert milliseconds to seconds
            if (player.playerDead) {
                gameSTATE = PLAYER_DEAD;
            }
            for (const auto& node : resourceNodes) {
                if (player.getCollisionBox().intersects(node.getCollisionBox())) {
                    // Handle collision with tree
                }
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

void render(SDL_Renderer* renderer,GAMESTATE& gameSTATE,Animation& splash,Player& player, Button& button1, Button& button2, Button& button3, Button& button4, float& deltaTime, int height, int width, std::vector<std::vector<int>>& map,std::vector<ResourceNode>& resourceNodes) {
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

                        //std::cout << map[y][x] << " ";
                        TextureManager::getInstance().renderTexture("groundTile",renderer,x*16,y*16,16,16);
                        /*
                        if(map[y][x] == 2)
                        {
                            //TextureManager::getInstance().renderTexture("tree1",renderer,(x*16)-16,(y*16)-16,32,32);
                            resourceNodes.emplace_back("tree1", renderer, (x*16)-16,(y*16)-16 , 32, 32);
                        }
                        else if(map[y][x] == 3)
                        {
                            //TextureManager::getInstance().renderTexture("bush",renderer,x*16,y*16,16,16);
                            resourceNodes.emplace_back("bush", renderer, x*16,y*16 , 16, 16);
                        }*/
                        //resourceNodes.emplace_back("rock", renderer, , , 16, 16);
                    }
                    //std::cout << std::endl;
                }
                // Render resource nodes
                for (auto& node : resourceNodes) {
                    node.render(renderer);
                }
                /*
                if(!cutSceneFinished){
                    TextureManager::getInstance().renderTexture("newGameCutScene",renderer,0,0,256,192);
                }*/

             player.render(renderer);
            break;
        case PLAYER_DEAD:
             //Render player death screen
             TextureManager::getInstance().renderTexture("gameover", renderer, (SCREEN_WIDTH/2)-50, 10, 100, 20);
            break;
        case OPTIONS_MENU:
            TextureManager::getInstance().renderTexture("optionsMenuBackground",renderer,0,0,256,192);
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
            if (noise > 0.1) {
                map[y][x] = 1; // Example: 1 for grass
            } else if (noise > 0.05) {
                map[y][x] = 2; // Example: 2 for tree
            } else {
                map[y][x] = 3; // Example: 3 for bush
            }
        }
    }
}
//std::map<int,std::map<int,int>>& tilemap
//std::map<int, int>>& tileMap
void generateTileMap(int width,int height,  std::vector<std::vector<int>>& map, std::map<int,std::map<int,int>>& tileMap){
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            int temp;
            temp = map[y][x];
            //tileMap[y][x] = temp;

                /*
            float noise = perlin(x * 0.1, y * 0.1); // Scale noise for larger maps
            if (noise > 0.1) {
                tileMap[y][x] = 1; // Example: 1 for grass
            } else if (noise > 0.05) {
                tileMap[y][x] = 2; // Example: 2 for tree
            } else {
                tileMap[y][x] = 3; // Example: 3 for bush
            }*/
        }
    }
}

int main(int argc, char* args[]) {
    const int width = 16;
    const int height = 12;

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
    SDL_SetWindowFullscreen( window, SDL_FALSE );
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

  //load sounds
  SoundManager::getInstance().loadSound("bonus", "sounds/bonus.wav");
  SoundManager::getInstance().loadSound("fail", "sounds/fail.wav");
  SoundManager::getInstance().loadSound("compute", "sounds/compute.wav");
  SoundManager::getInstance().loadSound("birdchirp", "sounds/birdchirp.wav");
  SoundManager::getInstance().loadSound("blunk", "sounds/blunk.wav");
  SoundManager::getInstance().loadSound("collect5", "sounds/collect5.wav");
  //load music
  auto& soundManager=SoundManager::getInstance();
  soundManager.loadMusic("build", "music/build.mp3");
  soundManager.loadMusic("continue", "music/continue.mp3");
  soundManager.loadMusic("midnight", "music/midnight.mp3");
  soundManager.loadMusic("strut", "music/strut.mp3");
    soundManager.loadMusic("progress", "music/gearsOfProgress.mp3");
  soundManager.loadMusic("upbeat", "music/upbeatLoop.mp3");
  soundManager.loadMusic("sand", "music/headInTheSand.mp3");

  soundManager.playRandomMusic();
  //load font
    TTF_Font* font = TTF_OpenFont("fonts/PublicPixel-z84yD.ttf", 8);
    if (font == nullptr) {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }
    //load textures
    TextureManager::getInstance().loadTexture("player", "images/ladyInBlue32x32.png", renderer);
    TextureManager::getInstance().loadTextTexture("gameover", "GAME OVER!!!", {255, 255, 255}, font, renderer);
    TextureManager::getInstance().loadTextTexture("ps3", "Project Shadow 3", {255, 255, 255}, font, renderer);
    TextureManager::getInstance().loadTexture("mmbg","images/mainMenuBackground.png",renderer);
    TextureManager::getInstance().loadTexture("titlebg","images/titleBG.png",renderer);
    TextureManager::getInstance().loadTexture("ground","images/ground.png",renderer);
    TextureManager::getInstance().loadTexture("bird","images/bird1.png",renderer);
    TextureManager::getInstance().loadTexture("bowl","images/bowl.png",renderer);
    TextureManager::getInstance().loadTexture("bowlSoup","images/bowlSoup.png",renderer);
    TextureManager::getInstance().loadTexture("bunny","images/bunny1.png",renderer);
    TextureManager::getInstance().loadTexture("images/bush1.png","images/bush1.png",renderer);
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
    TextureManager::getInstance().loadTexture("images/tree1.png","images/tree1.png",renderer);
    TextureManager::getInstance().loadTexture("tree2","images/tree2.png",renderer);
    TextureManager::getInstance().loadTexture("walnuts","images/walnuts.png",renderer);
    TextureManager::getInstance().loadTexture("groundTile","images/tiles/groundTile.png",renderer);
    TextureManager::getInstance().loadTexture("newGameCutScene","images/cutScenes/newGameCutSceneScaled256x192.png",renderer);
    TextureManager::getInstance().loadTexture("optionsMenuBackground","images/optionsMenuBackgroundScaled256x192.png",renderer);

    Button button1("images/buttons/new.png", "images/buttons/newMO.png", renderer, 10, 10, 100, 50);
    Button button2("images/buttons/load.png", "images/buttons/loadMO.png", renderer, 10, 70, 100, 50);
    Button button3("images/buttons/options.png", "images/buttons/optionsMO.png", renderer, 10, 130, 100, 50);
    Button button4("images/buttons/exit.png","images/buttons/exitMO.png",renderer,156,14,100,50);

    Player player("images/ladyInBlue16x16.png", renderer, (SCREEN_WIDTH/2)-8,(SCREEN_HEIGHT/2)-8);
    player.playerDead = false;

    // Initialize resource nodes
    std::vector<ResourceNode> resourceNodes;


    // Initialize tile map
    std::map<int, std::map<int, int>> tileMap;

    //initialize map
    std::vector<std::vector<int>> map(height, std::vector<int>(width, 0));

    //generate map
    generateMap(width, height, map);
    generateTileMap(width,height,map,tileMap);
    GameState gameState("save.dat");

    gameState.load(player);
    // Populate tileMap with initial values or load it
    gameState.loadTileMap(tileMap);

    Timer myTimer;
    myTimer.start();


    bool quit = false;
    SDL_Event e;
    Uint32 startTick, endTick;// deltaTime;
    float deltaTime;
    startTick = SDL_GetTicks();


    // Initialize animation with frame count and frame time
    Animation splash(256, 192, 80, 30); // Frame size: 256x192, 80 frames, 10 ms per frame
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


    while (!quit) {
        endTick = SDL_GetTicks();

        float deltaTime = static_cast<float>(endTick - startTick);
        startTick = endTick;
        while (SDL_PollEvent(&e) != 0) {
            handleEvents(e,gameSTATE,player,quit,button1,button2,button3,button4);
        }
        update(gameSTATE,splash,deltaTime,player,button1,button2,button3,button4,quit,height,width,map,tileMap,resourceNodes,renderer);
        render(renderer,gameSTATE,splash,player,button1,button2,button3,button4,deltaTime,height,width,map,resourceNodes);
        endTick = SDL_GetTicks();
        deltaTime = endTick - startTick;

        if (deltaTime < 15) {
            SDL_Delay(15 - deltaTime); // Cap the frame rate to ~60 FPS
        }

        Uint32 time = myTimer.getTicks();
        //std::cout << "Timer: " << time << " ms" << std::endl;
    }



    gameState.save(player);
    gameState.saveTileMap(tileMap);
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
