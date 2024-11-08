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
#include "Camera.h"
#include "enemy.h"
#include "particleSystem.h"

//socialist Americans don't pay income tax because they don't get representation.

//note, eventManager (eventManager seemed incomplete, ask Phaedra) and network have problems (network needs proper SDL libraries in place and linked).


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


//player interacts with a resourcenode
void handleInteraction(Player& player, std::vector<ResourceNode>& resourceNodes,std::vector<Enemy>& squirrels) {
    for (const auto& node : resourceNodes) {
        if (player.getInteractionBox().intersects(node.getInteractionBox())) {
            SoundManager::getInstance().playSound("collect2");
            /*
            if(node.getTextureId() == "Stone")
            {
                player.addItem(stone);
            }
            if(node.getTextureId() == "Branch")
            {
                //player.addItem(branch);
            }
            if(node.getTextureId() == "Walnuts")
            {
                player.addItem(walnuts);
            }*/
            // Handle other interactions, like collecting resources
        }
    }
    for(int i = 0; i < squirrels.size(); ++i)
    {
        if(player.getInteractionBox().intersects(squirrels[i].getCollisionBox()))
        {
            //enemy.dead();
            squirrels.erase(squirrels.begin() + i);
        }
    }

}

//Other necessary includes and initializations
void handleEvents(SDL_Event& e, GAMESTATE& gameSTATE, Player& player, bool& quit, Button& button1, Button& button2, Button& button3, Button& button4,Button& button5,SDL_Window* window,bool& fullSCREEN,bool& displayControls,bool& cutSceneFinished,std::vector<ResourceNode>& resourceNodes,GameState& gameState, std::map<int, std::map<int, int>>& tileMap,std::vector<Enemy>& squirrels) {
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
                if(gameSTATE == IN_GAME)
                {//if player is exiting IN_GAME gameSTATE, save the game.
                    gameState.save(player);
                    gameState.saveTileMap(tileMap);
                }
                gameSTATE = MAIN_MENU;
            }
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
        if(fullSCREEN)
        {
            SDL_SetWindowFullscreen( window, SDL_FALSE );
            fullSCREEN = false;
        }
        else
        {
            SDL_SetWindowFullscreen( window, SDL_TRUE );
            fullSCREEN = true;
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
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            if(!cutSceneFinished){
                cutSceneFinished = true;
            }
        }
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e) {
            handleInteraction(player, resourceNodes,squirrels);
        }
        player.handleEvents(e);
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)
        {
            player.decreaseHealth(101);
        }
        break;
    case PLAYER_DEAD:
        //player clicks to skip player dead screen.
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            gameSTATE = MAIN_MENU;
        }
        break;
    case OPTIONS_MENU:
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            mouseClicked = true;
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        button5.update(mouseX, mouseY, mouseClicked);
        break;
    }

     //Handle other events
}

void copyMap(const std::vector<std::vector<int>>& src, std::map<int, std::map<int, int>>& dest) {
    std::cout<<"\n map copied to tilemap!";
    for (size_t i = 0; i < src.size(); ++i) {
        for (size_t j = 0; j < src[i].size(); ++j) {
            dest[i][j] = src[i][j];
        }
    }
}

void copyTileMapToMap(const std::map<int, std::map<int, int>>& tileMap, std::vector<std::vector<int>>& map) {
    std::cout<<"\n tilemap copied to map";
    // Resize the vector to match the dimensions of the tileMap
    int maxRow = 0;
    int maxCol = 0;

    for (const auto& row : tileMap) {
        if (row.first > maxRow) {
            maxRow = row.first;
        }
        for (const auto& cell : row.second) {
            if (cell.first > maxCol) {
                maxCol = cell.first;
            }
        }
    }

    map.resize(maxRow + 1, std::vector<int>(maxCol + 1, 0));

    // Copy data from tileMap to map
    for (const auto& row : tileMap) {
        for (const auto& cell : row.second) {
            map[row.first][cell.first] = cell.second;
        }
    }
}

// Perlin Noise Function
float perlin(float x, float y) {
    // Implementation of Perlin noise
    // This is a placeholder. Replace it with a proper Perlin noise function
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

// Generate Procedural Map
void generateMap(int width, int height, std::vector<std::vector<int>>& map) {
    std::cout<<"\n map generated";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float noise = perlin(x * 0.1, y * 0.1); // Scale noise for larger maps
            if(noise > 0.18){
                map[y][x] = 1;
            }
            else if( noise > 0.15 ){
                map[y][x] = 7;
            }
            else if( noise > 0.12){
                map[y][x] = 2;
            }
            else if( noise > 0.9)
            {
                    map[y][x] = 3;
            }
            else if (noise > 0.6) {
                map[y][x] = 4; // Example: 1 for grass
            } else if (noise > 0.03) {
                map[y][x] = 5; // Example: 2 for tree
            } else {
                map[y][x] = 6; // Example: 3 for bush
            }
        }
    }
}

placeResourceNodes(int width, int height, std::vector<std::vector<int>>& map,SDL_Renderer* renderer,std::vector<ResourceNode>& resourceNodes){
    std::cout<<"\n placing resource nodes \n";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << map[y][x] << " ";
            if(map[y][x] == 1)
            {
                //resourceNodes.emplace_back("images/",renderer,x*16,y*16,16,16);
            }
            else if(map[y][x] == 2)
            {
                resourceNodes.emplace_back("images/tree1.png", renderer, (x*16)-16,(y*16)-16 , 32, 32);
            }
            else if(map[y][x] == 3)
            {
                resourceNodes.emplace_back("images/bush1.png", renderer, x*16,y*16 , 16, 16);
            }
            else if(map[y][x] == 4)
            {
                resourceNodes.emplace_back("images/rock1.png",renderer,x*16,y*16,16,16);
            }
            else if(map[y][x] == 5)
            {
                resourceNodes.emplace_back("images/stone1.png",renderer,x*16,y*16,16,16);
            }
            else if(map[y][x] == 6)
            {
                resourceNodes.emplace_back("images/walnuts.png",renderer,x*16,y*16,16,16);
            }
            else if(map[y][x] == 7)
            {
                resourceNodes.emplace_back("images/branch1.png",renderer,x*16,y*16,16,16);
            }
        }
        std::cout << std::endl;
    }
    return 0;
}

void update(GAMESTATE& gameSTATE, Animation& splash, float& deltaTime, Player& player, Button& button1, Button& button2, Button& button3, Button& button4,Button& button5,bool& quit, int height, int width, std::vector<std::vector<int>>& map, std::map<int, std::map<int, int>>& tileMap,std::vector<ResourceNode>& resourceNodes,SDL_Renderer* renderer,bool& displayControls, bool& cutSceneFinished,Timer& myTimer,Camera& camera,GameState& gameState, std::vector<Enemy>& squirrels,ParticleSystem& sparkles)
{
    int prevX, prevY;
    player.getPosition(prevX, prevY);
    switch (gameSTATE) {
        case SPLASH:
            // Your animation update logic
            splash.update(deltaTime);
            sparkles.update(deltaTime);
            if (splash.animationFinished) {
                gameSTATE = MAIN_MENU;
                SoundManager::getInstance().playSound("collect5");
            }
            break;
        case MAIN_MENU:
             //Logic for main menu
            if(button1.clicked)//newgame button clicked
            {
                gameSTATE = IN_GAME;
                //generate map
                generateMap(width, height, map);
                //set Player default starting position
                player.setPosition((SCREEN_WIDTH+(SCREEN_WIDTH/2)),(SCREEN_HEIGHT+(SCREEN_HEIGHT/2)));
                placeResourceNodes(width,height,map,renderer,resourceNodes);
                copyMap(map, tileMap);
                button1.clicked = false;
                SoundManager::getInstance().playSound("bonus");
                cutSceneFinished = false;
                myTimer.start();
            }
            if(button2.clicked)//loadgame button clicked
            {
                //prevent cutscene from playing on loadgame
                cutSceneFinished = true;
                //load player from save file
                gameState.load(player);
                // Populate tileMap with initial values or load it
                gameState.loadTileMap(tileMap);
                copyTileMapToMap(tileMap, map);
                placeResourceNodes(width,height,map,renderer,resourceNodes);
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
            if(cutSceneFinished){
                for(int i = 0; i < squirrels.size(); ++ i)
                {
                    squirrels[i].update( ( deltaTime / 1000.0f ) , player.getCollisionBox().getRect().x, player.getCollisionBox().getRect().y );
                }
            }
            player.update( ( deltaTime / 1000.0f ), camera.getCameraRect() ); // Convert milliseconds to seconds
            camera.update( player.getCollisionBox().getRect().x, player.getCollisionBox().getRect().y );

            if ( player.getHealth() <= 0 ) {
                gameSTATE = PLAYER_DEAD;
                player.setHealth(100);
            }
            for (const auto& node : resourceNodes) {
                if ( player.getCollisionBox().intersects(node.getCollisionBox()))  {
                    // If a collision is detected, revert the player's position to the previous position
                    player.setPosition(prevX, prevY);
                    break;
                }
            }
            for(int i = 0; i < squirrels.size(); ++i)
            {
                if( player.getCollisionBox().intersects(squirrels[i].getCollisionBox() ) ){
                    //if player collides with enemy
                    player.decreaseHealth(5);
                    player.setPosition(prevX, prevY);
                    break;
                }
            }
            break;
        case PLAYER_DEAD:
             //Logic for when player is dead
             resourceNodes.clear();
             //Possibly reset to MAIN_MENU or offer options for retry
            break;
        case OPTIONS_MENU:
            if(button5.clicked)
            {
                if(displayControls)
                {
                    displayControls = false;
                }
                else
                {
                    displayControls = true;
                }
                button5.clicked = false;
            }
            break;
    }
}

void render(SDL_Renderer* renderer,GAMESTATE& gameSTATE,Animation& splash,Player& player, Button& button1, Button& button2, Button& button3, Button& button4,Button& button5, float& deltaTime, int height, int width, std::vector<std::vector<int>>& map,std::vector<ResourceNode>& resourceNodes,bool& displayControls, bool& cutSceneFinished,Timer& myTimer,Camera& camera, std::vector<Enemy>& squirrels,ParticleSystem& sparkles) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    switch (gameSTATE) {
        case SPLASH:
             //Your rendering code for animation
             splash.render(renderer);
            sparkles.render(renderer);
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
                        TextureManager::getInstance().render("groundTile",renderer,x*16,y*16,16,16,camera.getCameraRect());
                    }
                }
                // Render resource nodes (behind player's texture)
                for (auto& node : resourceNodes) {
                    if ( node.getCollisionBox().getRect().y + node.getCollisionBox().getRect().h <= player.getCollisionBox().getRect().y + player.getCollisionBox().getRect().h ) {
                        //node.render(renderer);
                        SDL_Rect renderRect = node.getCollisionBox().getRect();
                        renderRect.x -= camera.getCameraRect().x;
                        renderRect.y -= camera.getCameraRect().y;
                        SDL_RenderCopy(renderer, node.getTexture(), nullptr, &renderRect);
                    }
                }
                if(!cutSceneFinished){
                    if(myTimer.getTicks()>=36000)
                    {
                        myTimer.stop();
                        cutSceneFinished = true;
                    }
                    if(myTimer.getTicks() < 36000)
                    {
                        TextureManager::getInstance().renderTexture("nightCampfire",renderer,0,0,256,192);
                        if(myTimer.getTicks() > 33000)
                        {
                            TextureManager::getInstance().renderTexture("CS12", renderer, 0, 170, 246, 20);
                        }
                        if(myTimer.getTicks() > 30000)
                        {
                            TextureManager::getInstance().renderTexture("CS11", renderer, 0, 140, 246, 20);
                        }
                        if(myTimer.getTicks() > 27500)
                        {
                            TextureManager::getInstance().renderTexture("CS10", renderer, 0, 110, 246, 20);
                        }
                    }
                    if(myTimer.getTicks() < 27000)
                    {
                        TextureManager::getInstance().renderTexture("newGameCutScene",renderer,0,0,256,192);
                        if(myTimer.getTicks() > 24000)
                        {
                            TextureManager::getInstance().renderTexture("CS9", renderer, 0, 170, 246, 20);
                        }
                        if(myTimer.getTicks() > 21000)
                        {
                            TextureManager::getInstance().renderTexture("CS8", renderer, 0, 140, 246, 20);
                        }
                        if(myTimer.getTicks() > 18500)
                        {
                            TextureManager::getInstance().renderTexture("CS7", renderer, 0, 110, 246, 20);
                        }
                    }
                    if(myTimer.getTicks() < 18000)
                    {

                        TextureManager::getInstance().renderTexture("teleport",renderer,0,0,256,192);
                        if(myTimer.getTicks() > 15000)
                        {
                            TextureManager::getInstance().renderTexture("CS6", renderer, 0, 170, 246, 20);
                        }
                        if(myTimer.getTicks() > 12000)
                        {
                            TextureManager::getInstance().renderTexture("CS5", renderer, 0, 140, 246, 20);
                        }
                        if(myTimer.getTicks() > 9500)
                        {
                            TextureManager::getInstance().renderTexture("CS4", renderer, 0, 110, 246, 20);
                        }
                    }
                    if(myTimer.getTicks() < 9000){
                        TextureManager::getInstance().renderTexture("shadowCouncil",renderer,0,0,256,192);
                        if(myTimer.getTicks() > 6000)
                        {
                            TextureManager::getInstance().renderTexture("CS3", renderer, 0, 170, 246, 20);
                        }
                        if(myTimer.getTicks() > 3000)
                        {
                            TextureManager::getInstance().renderTexture("CS2", renderer, 0, 140, 246, 20);
                        }
                        if(myTimer.getTicks() > 500)
                        {
                            TextureManager::getInstance().renderTexture("CS1", renderer, 0, 110, 246, 20);
                        }
                    }
                }
                if(cutSceneFinished){

                    for(int i = 0; i < squirrels.size(); ++i)
                    {
                        //squirrels[i].render(renderer,camera.getCameraRect());
                        SDL_Rect renderRect = squirrels[i].getCollisionBox().getRect();
                        renderRect.x -= camera.getCameraRect().x;
                        renderRect.y -= camera.getCameraRect().y;
                        SDL_RenderCopy(renderer, squirrels[i].getTexture(), nullptr, &renderRect);
                    }
                    SDL_Rect playerRect = player.getCollisionBox().getRect();
                    playerRect.x -= camera.getCameraRect().x;
                    playerRect.y -= camera.getCameraRect().y;
                    playerRect.x -= 4;
                    playerRect.y -= 8;
                    playerRect.w = 16;
                    playerRect.h = 16;
                    SDL_RenderCopy(renderer, player.getTexture(), nullptr, &playerRect);
                    for (auto& node : resourceNodes) {
                        if (  node.getCollisionBox().getRect().y + node.getCollisionBox().getRect().h > player.getCollisionBox().getRect().y + player.getCollisionBox().getRect().h  ) {
                            SDL_Rect renderRect = node.getCollisionBox().getRect();
                            renderRect.x -= camera.getCameraRect().x;
                            renderRect.y -= camera.getCameraRect().y;
                            SDL_RenderCopy(renderer, node.getTexture(), nullptr, &renderRect);
                        }
                    }
                    TextureManager::getInstance().renderTexture("HUD",renderer,0,0,256,192);

                    // The code below renders inventoryItem icons.  implement at HUD and when player viewing inventory.

                    //int xx, yy;
                    //xx = 130;
                    //yy = 166;
                    //player.getPosition(xx,yy);
                    /*
                    for(int i = player.getInventory().size()-1; i >= 0; i--){
                        //std::cout<<"\n index: "<<i<<" itemName: "<<player.getInventory()[i].getName();
                        std::cout<<"\n index: "<<i<<" iconTextureId: "<<player.getInventory()[i].getIconTextureId();
                    }*/
                    for(int i = 0; i < player.getInventory().size(); ++i)
                    {
                        player.getInventory()[i].renderIcon(renderer,((i*16) + 64),174);
                        //player.getInventory()[1].renderIcon(renderer,80,174);
                        //player.getInventory()[2].renderIcon(renderer,92,174);
                        //player.getInventory()[3].renderIcon(renderer,105,174);
                    }
                }

            break;
        case PLAYER_DEAD:
             //Render player death screen
             TextureManager::getInstance().renderTexture("gameover", renderer, (SCREEN_WIDTH/2)-50, 10, 100, 20);
            break;
        case OPTIONS_MENU:
            TextureManager::getInstance().renderTexture("optionsMenuBackground",renderer,0,0,256,192);
            button5.render(renderer);
            if(displayControls)
            {
                TextureManager::getInstance().renderTexture("w", renderer, 5, 80, 40, 10);
                TextureManager::getInstance().renderTexture("a", renderer, 5, 95, 45, 10);
                TextureManager::getInstance().renderTexture("s", renderer, 5, 110, 45, 10);
                TextureManager::getInstance().renderTexture("d", renderer, 5, 125, 46, 10);
                TextureManager::getInstance().renderTexture("ESC", renderer, 5, 140, 246, 10);
                TextureManager::getInstance().renderTexture("e", renderer, 5, 155, 95, 10);
                TextureManager::getInstance().renderTexture("f", renderer, 5, 170, 75, 10);
                TextureManager::getInstance().renderTexture("lmb", renderer, 5, 65, 176, 10);
            }
            break;
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    const int width = 48;//by tiles
    const int height = 36;

    int mapWidth = width*16; // by pixels
    int mapHeight = height*16;

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
  SoundManager::getInstance().loadSound("collect2", "sounds/collect2.wav");
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
    //load text textures
    TextureManager::getInstance().loadTextTexture("gameover", "GAME OVER!!!", {255, 255, 255}, font, renderer);
    TextureManager::getInstance().loadTextTexture("ps3", "Project Shadow 3", {255, 255, 255}, font, renderer);

    TextureManager::getInstance().loadTextTexture("CS1","Every year on the winter solstice",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("CS2","the shadow council meets to discuss",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("CS3","the future of humanity.",{255,0,0},font,renderer);

    TextureManager::getInstance().loadTextTexture("CS4","Earth's ecosystem is doomed.",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("CS5","So the shadow council teleports",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("CS6","hand-picked humans off-world.",{255,0,0},font,renderer);

    TextureManager::getInstance().loadTextTexture("CS7","They decide who gets sent to",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("CS8","Proxima Centauri b as part of 'Project Shadow'",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("CS9","which is the best hope of humanity's survival.",{255,0,0},font,renderer);

    TextureManager::getInstance().loadTextTexture("CS10","Your mission, whether you like it or not,",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("CS11","is to colonize Centauri b.",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("CS12","Survive any way you can.",{255,0,0},font,renderer);

    //options screen controls display text textures
    TextureManager::getInstance().loadTextTexture("w","w: up",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("a","a: left",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("s","s: down",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("d","d: right",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("ESC","ESCAPE KEY: return to main menu, exit game at main menu",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("e","e: interact with object",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("f","f: toggle fullscreen",{255,0,0},font,renderer);
    TextureManager::getInstance().loadTextTexture("lmb","left mouse button: advance cutscenes",{255,0,0},font,renderer);

    //load textures
    TextureManager::getInstance().loadTexture("player", "images/ladyInBlue32x32.png", renderer);
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
    //TextureManager::getInstance().loadTexture("rock","images/rock1.png",renderer);
    TextureManager::getInstance().loadTexture("squirrel","images/squirrel1.png",renderer);
    TextureManager::getInstance().loadTexture("sun","images/sun1.png",renderer);
    //TextureManager::getInstance().loadTexture("images/tree1.png","images/tree1.png",renderer);
    TextureManager::getInstance().loadTexture("tree2","images/tree2.png",renderer);

    TextureManager::getInstance().loadTexture("groundTile","images/tiles/groundTile.png",renderer);
    TextureManager::getInstance().loadTexture("newGameCutScene","images/cutScenes/newGameCutSceneScaled256x192.png",renderer);
    TextureManager::getInstance().loadTexture("shadowCouncil","images/cutScenes/shadowCouncilScaled256x192.png",renderer);
    TextureManager::getInstance().loadTexture("teleport","images/cutScenes/teleportScaled256x192.png",renderer);
    TextureManager::getInstance().loadTexture("nightCampfire","images/cutScenes/nightCampfireScaled256x192.png",renderer);
    TextureManager::getInstance().loadTexture("optionsMenuBackground","images/optionsMenuBackgroundScaled256x192.png",renderer);
    TextureManager::getInstance().loadTexture("HUD","images/HUD/HUD.png",renderer);
    TextureManager::getInstance().setAlpha("HUD", 204); // 204 is 80% of 255

    Button button1("images/buttons/new.png", "images/buttons/newMO.png", renderer, 10, 10, 100, 50);
    Button button2("images/buttons/load.png", "images/buttons/loadMO.png", renderer, 10, 70, 100, 50);
    Button button3("images/buttons/options.png", "images/buttons/optionsMO.png", renderer, 10, 130, 100, 50);
    Button button4("images/buttons/exit.png","images/buttons/exitMO.png",renderer,156,14,100,50);
    Button button5("images/buttons/controls.png","images/buttons/controlsMO.png",renderer,25,25,100,50);

    Player player("images/ladyInBlue16x16.png", renderer, (SCREEN_WIDTH+(SCREEN_WIDTH/2)),(SCREEN_HEIGHT+(SCREEN_HEIGHT/2)));
    Camera camera(256, 192,mapWidth,mapHeight);

    std::vector<Enemy> squirrels;//("images/squirrel1.png",renderer,100,100);
    int squirrelMax = 8; //std::rand() % 8;
    int squirrelX;
    int squirrelY;
    //squirrelMax += 1;
    std::cout<<"\n squirrelMax = "<<squirrelMax<<"\n";

    std::srand(std::time(0));
    squirrelX = std::rand() % 768;
    squirrelY = std::rand() % 576;
    for(int i = 0; i < squirrelMax; ++i)
    {

        std::cout<<"\n squirrelX = "<<squirrelX;
        std::cout<<"\n squirrelY = "<<squirrelY<<"\n";
        squirrels.emplace_back("images/squirrel1.png",renderer,squirrelX,squirrelY);
        std::srand(squirrelX);
        squirrelX = std::rand() % 768;
        squirrelY = std::rand() % 576;
    }

    //resourceNodes.emplace_back("images/",renderer,x*16,y*16,16,16);
    //resourceNodes.emplace_back("images/",renderer,x*16,y*16,16,16);


    ParticleSystem sparkles(100);
    //sparkles.createParticles();


    bool fullSCREEN;
    fullSCREEN = false;

    // Initialize resource nodes
    std::vector<ResourceNode> resourceNodes;


    // Initialize tile map
    std::map<int, std::map<int, int>> tileMap;

    //initialize map
    std::vector<std::vector<int>> map(height, std::vector<int>(width, 0));



    GameState gameState("save.dat");

    Timer myTimer;


    bool cutSceneFinished;
    bool displayControls;
    displayControls = false;

    bool quit = false;
    SDL_Event e;
    Uint32 startTick, endTick;// deltaTime;
    //float deltaTime;
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

    // Initialize items with various properties
    InventoryItem sword(renderer,"Sword", "A sharp blade used for combat.","images/icons/sword.png", InventoryItem::EQUIPPABLE, true, false, -1);
    InventoryItem potion(renderer,"Potion", "A healing potion for emergencies.","images/icons/potion.png", InventoryItem::SOLID, false, true);
    InventoryItem stone(renderer,"Stone","Smaller than a boulder, larger than a pebble.","images/stone1.png",InventoryItem::SOLID,false,false,-1,false,false,5,-1);
    InventoryItem walnuts(renderer,"Walnuts","Shells like walnuts, taste like walnuts.","images/walnuts.png",InventoryItem::SOLID,false,true,-1,false,false,1,-1,InventoryItem::FRESH);
        //TextureManager::getInstance().loadTexture("walnuts","images/walnuts.png",renderer);
    /*
    InventoryItem bandage("Bandage", "Used to treat wounds.", InventoryItem::SOLID, false, false, 3);
    InventoryItem bottle("Bottle", "Can hold liquids.", InventoryItem::SOLID, false, false, -1, true, false, 0, 100); // Max volume of 100
    InventoryItem box("Box", "Can hold solid items.", InventoryItem::SOLID, false, false, -1, false, true, 0, 50); // Max volume of 50
    InventoryItem water("Water", "Essential for life.", InventoryItem::LIQUID, false, true, 1, true, false, 10); // Volume of 10
    InventoryItem rock("Rock", "A solid object.", InventoryItem::SOLID, false, true, 1, false, true, 20); // Volume of 20
    */


    // Add items to the player's inventory
    //player.addItem(sword);
    //player.addItem(potion);
    //player.addItem(stone);
    //player.addItem(walnuts);
    /*
    player.addItem(bandage);
    player.addItem(bottle);
    player.addItem(box);
    player.addItem(water);
    player.addItem(rock);*/


    while (!quit) {
        endTick = SDL_GetTicks();

        float deltaTime = static_cast<float>(endTick - startTick);
        startTick = endTick;
        while (SDL_PollEvent(&e) != 0) {
            handleEvents(e,gameSTATE,player,quit,button1,button2,button3,button4,button5,window,fullSCREEN,displayControls,cutSceneFinished,resourceNodes,gameState,tileMap,squirrels);

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_e) {
                    //player.equipItem("Sword"); // Equip the sword when 'e' is pressed
                } else if (e.key.keysym.sym == SDLK_u) {
                    //player.useItem("Potion"); // Use the potion when 'u' is pressed
                    //player.useItem("Bandage"); // Use the bandage when 'u' is pressed
                } else if (e.key.keysym.sym == SDLK_c) {
                    /*
                    player.addContainedItem("Bottle", water); // Add water to bottle when 'c' is pressed
                    player.addContainedItem("Box", rock); // Add rock to box when 'c' is pressed
                    */
                } else if (e.key.keysym.sym == SDLK_r) {
                    /*
                    player.removeContainedItem("Bottle", "Water"); // Remove water from bottle when 'r' is pressed
                    player.removeContainedItem("Box", "Rock"); // Remove rock from box when 'r' is pressed
                    */
                }
            }
        }

        update(gameSTATE,splash,deltaTime,player,button1,button2,button3,button4,button5,quit,height,width,map,tileMap,resourceNodes,renderer,displayControls,cutSceneFinished,myTimer,camera,gameState,squirrels,sparkles);
        player.updateItemsState();
        render(renderer,gameSTATE,splash,player,button1,button2,button3,button4,button5,deltaTime,height,width,map,resourceNodes,displayControls,cutSceneFinished,myTimer,camera,squirrels,sparkles);


        endTick = SDL_GetTicks();
        deltaTime = endTick - startTick;

        if (deltaTime < 15) {
            SDL_Delay(15 - deltaTime); // Cap the frame rate to ~60 FPS
        }

        //Uint32 time = myTimer.getTicks();
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
