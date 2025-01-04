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

//note, eventManager (eventManager seemed incomplete, ask Phaedra) and network class has problems (network needs proper SDL libraries in place and linked).

//enemy texture is invisible? sometimes.  their collisionbox still exists and can kill the player.

//need slider for volume, and back button for options_menu gamestate.

//allow player to click items in inventory or hotbar to use them.
//allow player to move inventory items to the hotbar if they press 1-8 while mouse is hovering over the item.
//maybe allow player to move items from the hotbar to the inventory by right clicking them.

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
void handleInteraction(Player& player, std::vector<ResourceNode>& resourceNodes,std::vector<Enemy>& squirrels,std::vector<Enemy>& bunnies, InventoryItem& stone, InventoryItem& branch, InventoryItem& walnuts) {
    //for (const auto& node : resourceNodes) {
    for (int i = 0; i < resourceNodes.size(); i++)//iterate through nodes
    {
        if (player.getInteractionBox().intersects(resourceNodes[i].getInteractionBox()))//if player is within interaction range of node
        {
            if(resourceNodes[i].getTextureId() == "images/walnuts.png")//if it's a walnut
            {

                if(!player.hotBarFull())
                {
                    //std::cout<<"\n first empty slot = "<< player.firstEmptySlot();
                    int slot = player.firstEmptySlot();
                    //player.addHotBarItem(walnuts,slot);
                    player.addHotBarItem(walnuts,player.firstEmptySlot());
                    resourceNodes.erase(resourceNodes.begin()+i);
                    SoundManager::getInstance().playSound("collect2");
                }
                //deprecated by above
                /*
                if(player.getHotBarSize() < player.getMaxHotBarSize()){//if hotbar isn't full
                    player.addHotBarItem(walnuts);
                    std::cout<<"\n player.hotBarFull() = "<<player.hotBarFull();
                    resourceNodes.erase(resourceNodes.begin()+i);
                    SoundManager::getInstance().playSound("collect2");
                }*/

                else{
                    if(!player.inventoryFull())//if hotbar is full, but inventory isn't
                    {
                        player.addItem(walnuts,player.firstEmptyInventorySlot());
                        resourceNodes.erase(resourceNodes.begin()+i);
                        SoundManager::getInstance().playSound("collect2");
                    }
                }
            }
            else
            {
                if(resourceNodes[i].getTextureId() == "images/stone1.png")
                {
                    if(!player.inventoryFull())
                    {
                        //player.addItem(stone);
                        player.addItem(stone,player.firstEmptyInventorySlot());
                        resourceNodes.erase(resourceNodes.begin()+i);
                        SoundManager::getInstance().playSound("collect2");
                    }
                }
                if(resourceNodes[i].getTextureId() == "images/branch1.png")
                {
                    if(!player.inventoryFull())
                    {
                        //player.addItem(branch);
                        player.addItem(branch,player.firstEmptyInventorySlot());
                        resourceNodes.erase(resourceNodes.begin()+i);
                        SoundManager::getInstance().playSound("collect2");
                    }
                }
            }
            // Handle other interactions, like collecting resources
        }
    }
    for(int i = 0; i < squirrels.size(); ++i)
    {
        if(player.getInteractionBox().intersects(squirrels[i].getCollisionBox()))
        {
            //enemy.dead();
            squirrels.erase(squirrels.begin() + i);
            std::cout<<"\n erasing squirrel #"<<i;
            SoundManager::getInstance().playSound("bwah");
        }
    }
    for(int i = 0; i<bunnies.size(); ++i)
    {
        if(player.getInteractionBox().intersects(bunnies[i].getCollisionBox()))
        {
            bunnies.erase(bunnies.begin()+i);
            std::cout<<"\n erasing bunny #"<<i;
            SoundManager::getInstance().playSound("bwah");
        }
    }

}

//Other necessary includes and initializations
void handleEvents(SDL_Event& e, GAMESTATE& gameSTATE, Player& player, bool& quit, Button& button1, Button& button2, Button& button3, Button& button4,Button& button5,Button& button6,Button& button7,Button& button8,SDL_Window* window,bool& fullSCREEN,bool& displayControls,bool& cutSceneFinished,std::vector<ResourceNode>& resourceNodes,GameState& gameState, std::map<int, std::map<int, int>>& tileMap,std::vector<Enemy>& squirrels,std::vector<Enemy>& bunnies, InventoryItem& stone, InventoryItem& branch, InventoryItem& walnuts, InventoryItem& emptySlot,bool& inventoryMenu) {
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
    bool mouseOver = false;
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
        if(fullSCREEN){mouseX-=24;}
        button1.update(mouseX, mouseY, mouseClicked);
        button2.update(mouseX, mouseY, mouseClicked);
        button3.update(mouseX, mouseY, mouseClicked);
        button4.update(mouseX, mouseY, mouseClicked);
        break;
    case IN_GAME:
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            mouseClicked = true;
        }

        if(!cutSceneFinished){
            cutSceneFinished = true;
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        if(fullSCREEN){mouseX-=24;}//fix for aspect ratio pixels
        button6.update(mouseX, mouseY, mouseClicked);
        //button7.update(mouseX,mouseY, mouseClicked);
        //button8.update(mouseX,mouseY,mouseClicked);

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e) {
            handleInteraction(player, resourceNodes,squirrels,bunnies,stone,branch,walnuts);
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_i){
            if(!inventoryMenu){
                inventoryMenu = true;
            }else{
                inventoryMenu = false;
            }

        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_1  ){//player pressed "1" button
            if(player.getHotBar()[0].getName() == emptySlot.getName())//hotbar slot 1 is empty
            {
                if(inventoryMenu)//inventory menu is open (just like the glory hole).
                {
                    for(int i =0; i < player.getInventory().size(); i++)
                    {
                        //if(mouse pointer collides with a usable item in the inventory (currently only walnuts), move that item to first hotbar slot.
                        /*
                        player.getInventory()[i].update(mouseX,mouseY,mouseClicked); //inventoryItem is const, so running update is making trouble.
                        if(player.getInventory()[i].hover){
                            player.addHotBarItem(player.getInventory()[i],0);//insert inventory item into hotbar slot 0
                            player.addItem(emptySlot,i);
                        }*/
                    }

                }
            }
            if(player.getHotBar()[0].getName() == walnuts.getName()){
                if(player.getHealth() < 100)
                {
                    if(player.getHealth() < 90)
                    {
                        player.increaseHealth(10);
                    }
                    else
                    {
                        player.setHealth(100);
                    }
                }
                if(player.getHunger()<100)
                {
                    if(player.getHunger() < 90)
                    {
                        player.increaseHunger(10);
                    }
                    else
                    {
                        player.setHunger(100);
                    }
                }
                SoundManager::getInstance().playSound("powerUp");
                player.addHotBarItem(emptySlot,0);
            }
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_2)
        {
            if(player.getHotBar()[1].getName() == walnuts.getName()){
                if(player.getHealth() < 100)
                {
                    if(player.getHealth() < 90)
                    {
                        player.increaseHealth(10);
                    }
                    else
                    {
                        player.setHealth(100);
                    }
                }
                if(player.getHunger()<100)
                {
                    if(player.getHunger() < 90)
                    {
                        player.increaseHunger(10);
                    }
                    else
                    {
                        player.setHunger(100);
                    }
                }
                SoundManager::getInstance().playSound("powerUp");
                player.addHotBarItem(emptySlot,1);
            }
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_3)
        {
            if(player.getHotBar()[2].getName() == walnuts.getName()){
                if(player.getHealth() < 100)
                {
                    if(player.getHealth() < 90)
                    {
                        player.increaseHealth(10);
                    }
                    else
                    {
                        player.setHealth(100);
                    }
                }
                if(player.getHunger()<100)
                {
                    if(player.getHunger() < 90)
                    {
                        player.increaseHunger(10);
                    }
                    else
                    {
                        player.setHunger(100);
                    }
                }
                SoundManager::getInstance().playSound("powerUp");
                player.addHotBarItem(emptySlot,2);
            }
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_4)
        {
            if(player.getHotBar()[3].getName() == walnuts.getName()){
                if(player.getHealth() < 100)
                {
                    if(player.getHealth() < 90)
                    {
                        player.increaseHealth(10);
                    }
                    else
                    {
                        player.setHealth(100);
                    }
                }
                if(player.getHunger()<100)
                {
                    if(player.getHunger() < 90)
                    {
                        player.increaseHunger(10);
                    }
                    else
                    {
                        player.setHunger(100);
                    }
                }
                SoundManager::getInstance().playSound("powerUp");
                player.addHotBarItem(emptySlot,3);
            }
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_5)
        {
            if(player.getHotBar()[4].getName() == walnuts.getName()){
                if(player.getHealth() < 100)
                {
                    if(player.getHealth() < 90)
                    {
                        player.increaseHealth(10);
                    }
                    else
                    {
                        player.setHealth(100);
                    }
                }
                if(player.getHunger()<100)
                {
                    if(player.getHunger() < 90)
                    {
                        player.increaseHunger(10);
                    }
                    else
                    {
                        player.setHunger(100);
                    }
                }
                SoundManager::getInstance().playSound("powerUp");
                player.addHotBarItem(emptySlot,4);
            }
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_6)
        {
            if(player.getHotBar()[5].getName() == walnuts.getName()){
                if(player.getHealth() < 100)
                {
                    if(player.getHealth() < 90)
                    {
                        player.increaseHealth(10);
                    }
                    else
                    {
                        player.setHealth(100);
                    }
                }
                if(player.getHunger()<100)
                {
                    if(player.getHunger() < 90)
                    {
                        player.increaseHunger(10);
                    }
                    else
                    {
                        player.setHunger(100);
                    }
                }
                SoundManager::getInstance().playSound("powerUp");
                player.addHotBarItem(emptySlot,5);
            }
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_7)
        {
            if(player.getHotBar()[6].getName() == walnuts.getName()){
                if(player.getHealth() < 100)
                {
                    if(player.getHealth() < 90)
                    {
                        player.increaseHealth(10);
                    }
                    else
                    {
                        player.setHealth(100);
                    }
                }
                if(player.getHunger()<100)
                {
                    if(player.getHunger() < 90)
                    {
                        player.increaseHunger(10);
                    }
                    else
                    {
                        player.setHunger(100);
                    }
                }
                SoundManager::getInstance().playSound("powerUp");
                player.addHotBarItem(emptySlot,6);
            }
        }
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_8)
        {
            if(player.getHotBar()[7].getName() == walnuts.getName()){
                if(player.getHealth() < 100)
                {
                    if(player.getHealth() < 90)
                    {
                        player.increaseHealth(10);
                    }
                    else
                    {
                        player.setHealth(100);
                    }
                }
                if(player.getHunger()<100)
                {
                    if(player.getHunger() < 90)
                    {
                        player.increaseHunger(10);
                    }
                    else
                    {
                        player.setHunger(100);
                    }
                }
                SoundManager::getInstance().playSound("powerUp");
                player.addHotBarItem(emptySlot,7);
            }
        }

        player.handleEvents(e);

        /*
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)
        {//this button is just to test player death
            player.decreaseHealth(101);
        }*/
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
    //copies map to tilemap
    for (size_t i = 0; i < src.size(); ++i) {
        for (size_t j = 0; j < src[i].size(); ++j) {
            dest[i][j] = src[i][j];
        }
    }
}

void copyTileMapToMap(const std::map<int, std::map<int, int>>& tileMap, std::vector<std::vector<int>>& map) {
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

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if(map[y][x] == 1)
            {
                //resourceNodes.emplace_back("images/",renderer,x*16,y*16,16,16);
            }
            else if(map[y][x] == 2)
            {
                resourceNodes.emplace_back("images/tree1.png", renderer, (x*16),(y*16) , 32, 32);
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
    }
    return 0;
}

void update(GAMESTATE& gameSTATE, Animation& splash, float& deltaTime, Player& player, Button& button1, Button& button2, Button& button3, Button& button4,Button& button5,Button& button6,Button& button7,Button& button8,bool& quit, int height, int width, std::vector<std::vector<int>>& map, std::map<int, std::map<int, int>>& tileMap,std::vector<ResourceNode>& resourceNodes,SDL_Renderer* renderer,bool& displayControls, bool& cutSceneFinished,Timer& myTimer,Camera& camera,GameState& gameState, std::vector<Enemy>& squirrels, std::vector<Enemy>& bunnies,ParticleSystem& sparkles,int& currentThirstBarPixels,int& currentHungerBarPixels,int& currentHealthBarPixels,bool& inventoryMenu)
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
                for(int i = 0; i < bunnies.size(); ++i)
                {
                    bunnies[i].update((deltaTime/1000.0f),player.getCollisionBox().getRect().x,player.getCollisionBox().getRect().y);
                }
                if(button6.clicked)//newgame button clicked
                {
                    if(!inventoryMenu)
                    {
                        inventoryMenu = true;
                    }
                    else
                    {
                        inventoryMenu = false;
                    }
                    button6.clicked = false;
                }
                player.update( ( deltaTime / 1000.0f ), camera.getCameraRect() ); // Convert milliseconds to seconds
            }

            camera.update( player.getCollisionBox().getRect().x, player.getCollisionBox().getRect().y );

            currentThirstBarPixels = (int)(.59*player.getThirst());
            currentHungerBarPixels = (int)(.59*player.getHunger());
            currentHealthBarPixels = (int)(.59*player.getHealth());

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
                    player.decreaseHealth(1);
                    SoundManager::getInstance().playSound("ouch");
                    player.setPosition(prevX, prevY);
                    break;
                }
            }
            for(int i = 0; i<bunnies.size(); ++i)
            {
                if(player.getCollisionBox().intersects(bunnies[i].getCollisionBox())){

                    player.decreaseHealth(1);
                    SoundManager::getInstance().playSound("ouch");
                    player.setPosition(prevX,prevY);
                }
            }
            break;
        case PLAYER_DEAD:
             //Logic for when player is dead
             resourceNodes.clear();
             for(int i = 0; i < player.getInventory().size(); i++)
            {
                player.removeItemFromInventory();
            }
            for(int j = 0; j < player.getHotBar().size(); j++){
                player.removeItemFromHotBar(j);
            }
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

void render(SDL_Renderer* renderer,GAMESTATE& gameSTATE,Animation& splash,Player& player, Button& button1, Button& button2, Button& button3, Button& button4,Button& button5,Button& button6,Button& button7,Button& button8, float& deltaTime, int height, int width, std::vector<std::vector<int>>& map,std::vector<ResourceNode>& resourceNodes,bool& displayControls, bool& cutSceneFinished,Timer& myTimer,Camera& camera, std::vector<Enemy>& squirrels,std::vector<Enemy>& bunnies,ParticleSystem& sparkles,int& currentThirstBarPixels,int& currentHungerBarPixels,int& currentHealthBarPixels,bool& inventoryMenu) {
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
                        if(node.getTextureId() == "images/tree1.png")
                        {
                            renderRect.y -=16;

                            renderRect.h = 32;
                            renderRect.w = 32;
                        }
                        else
                        {
                            renderRect.h = 16;
                            renderRect.w = 16;
                        }
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

                    for(int i = 0; i < squirrels.size(); ++i)//squirrels sometimes render as black boxes.
                    {
                        //squirrels[i].render(renderer,camera.getCameraRect());
                        SDL_Rect renderRect = squirrels[i].getCollisionBox().getRect();
                        renderRect.x -= camera.getCameraRect().x;
                        renderRect.y -= camera.getCameraRect().y;
                        renderRect.w = 16;
                        renderRect.h = 16;
                        SDL_RenderCopy(renderer, squirrels[i].getTexture(), nullptr, &renderRect);
                    }
                    for(int i = 0; i < bunnies.size(); ++i)
                    {
                        SDL_Rect renderRect = bunnies[i].getCollisionBox().getRect();
                        renderRect.x -= camera.getCameraRect().x;
                        renderRect.y -= camera.getCameraRect().y;
                        renderRect.w = 16;
                        renderRect.h = 16;
                        SDL_RenderCopy(renderer, bunnies[i].getTexture(), nullptr, &renderRect);
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
                            if(node.getTextureId() == "images/tree1.png")
                            {
                                renderRect.y -=16;
                                renderRect.h = 32;
                                renderRect.w = 32;
                            }
                            else
                            {
                                renderRect.h = 16;
                                renderRect.w = 16;
                            }
                            SDL_RenderCopy(renderer, node.getTexture(), nullptr, &renderRect);
                        }
                    }
                    TextureManager::getInstance().renderTexture("HUD",renderer,0,0,256,192);
                    //button7.render(renderer);
                    //button8.render(renderer);
                    //std::cout<<"\n currentThirstBarPixels: "<<currentThirstBarPixels;
                    TextureManager::getInstance().renderTexture("thirstBar",renderer,194,3,currentThirstBarPixels,5);
                    TextureManager::getInstance().renderTexture("hungerBar",renderer,194,9,currentHungerBarPixels,5);
                    TextureManager::getInstance().renderTexture("healthBar",renderer,194,15,currentHealthBarPixels,5);
                    button6.render(renderer);//the inventory button in the lower left corner of the screen.
                    if(inventoryMenu)
                    {
                        TextureManager::getInstance().renderTexture("inventoryMenu",renderer,5,59,178,93);
                    }

                        for(int i = 0; i < player.getHotBar().size(); i++)
                        {
                            int tempX = player.getHotBarItemX(i);
                            //hotbar items

                            player.getHotBar()[i].renderIcon(renderer,tempX,174);

                        }

                        for(int i = 0; i < player.getInventory().size(); i++)//iterates through inventory to render item icons.
                        {
                            if(inventoryMenu)//inventory menu is open (player pressed 'i' or clicked backpack button in HUD
                            {
                                if(i<8)//each 8 iterations is a row in the inventory, i%8 is column number.
                                {
                                    player.getInventory()[i].renderIcon(renderer,39 + (i*17),83);
                                }
                                if(i>=8 && i < 16)
                                {
                                    player.getInventory()[i].renderIcon(renderer,39 + ((i-8)*17),99);
                                }
                                if(i>=16 && i < 24)
                                {
                                    player.getInventory()[i].renderIcon(renderer,39 + ((i-16)*17),115);
                                }
                                if(i>=24 && i < 32)
                                {
                                    player.getInventory()[i].renderIcon(renderer,39 + ((i-24)*17),131);
                                }
                                if(i>=32){
                                    std::cout<<"\n player has greater than 32 items in inventory!";
                                }
                            }
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
                TextureManager::getInstance().renderTexture("i", renderer, 5, 50, 95, 10);
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
    SDL_Window* window = SDL_CreateWindow("Project Shadow 3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 192, SDL_WINDOW_SHOWN);//SDL_WINDOW_FULLSCREEN_DESKTOP);
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
  SoundManager::getInstance().loadSound("bwah","sounds/bwah.wav");
  SoundManager::getInstance().loadSound("ouch","sounds/ouch.wav");
  SoundManager::getInstance().loadSound("powerUp","sounds/Powerup.wav");

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
    TextureManager::getInstance().loadTextTexture("i","open/close Inventory",{255,0,0},font,renderer);

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

    TextureManager::getInstance().loadTexture("thirstBar","images/HUD/thirstBar.png",renderer);
    TextureManager::getInstance().setAlpha("thirstBar", 204); // 204 is 80% of 255

    TextureManager::getInstance().loadTexture("hungerBar","images/HUD/hungerBar.png",renderer);
    TextureManager::getInstance().setAlpha("hungerBar", 204); // 204 is 80% of 255

    TextureManager::getInstance().loadTexture("healthBar","images/HUD/healthBar.png",renderer);
    TextureManager::getInstance().setAlpha("healthBar", 204); // 204 is 80% of 255

    TextureManager::getInstance().loadTexture("inventoryMenu","images/HUD/inventoryMenu.png",renderer);
    TextureManager::getInstance().setAlpha("inventoryMenu",204);

    Button button1("images/buttons/new.png", "images/buttons/newMO.png", renderer, 10, 10, 100, 50);
    Button button2("images/buttons/load.png", "images/buttons/loadMO.png", renderer, 10, 70, 100, 50);
    Button button3("images/buttons/options.png", "images/buttons/optionsMO.png", renderer, 10, 130, 100, 50);
    Button button4("images/buttons/exit.png","images/buttons/exitMO.png",renderer,156,14,100,50);
    Button button5("images/buttons/controls.png","images/buttons/controlsMO.png",renderer,5,5,100,50);
    Button button6("images/buttons/inventory.png","images/buttons/inventoryMO.png",renderer,0,152,32,36);
    Player player("images/ladyInBlue16x16.png", renderer, (SCREEN_WIDTH+(SCREEN_WIDTH/2)),(SCREEN_HEIGHT+(SCREEN_HEIGHT/2)));
    Camera camera(256, 192,mapWidth,mapHeight);

    std::vector<Enemy> squirrels;//("images/squirrel1.png",renderer,100,100);
    std::vector<Enemy> bunnies;
    int bunniesMax = 8;
    int bunnyX;
    int bunnyY;
    int squirrelMax = 8; //std::rand() % 8;
    int squirrelX;
    int squirrelY;
    //squirrelMax += 1;
    //std::cout<<"\n squirrelMax = "<<squirrelMax<<"\n";

    std::srand(std::time(0));
    bunnyX = std::rand() % 768;
    bunnyY = std::rand() % 576;
    squirrelX = std::rand() % 768;
    squirrelY = std::rand() % 576;

    for(int i = 0; i < bunniesMax; ++i)
    {
        bunnies.emplace_back("images/bunny1.png",renderer,bunnyX,bunnyY);
        std::srand(bunnyX);
        bunnyX = std::rand() % 768;
        bunnyY = std::rand() % 576;
    }
    for(int i = 0; i < squirrelMax; ++i)
    {
        squirrels.emplace_back("images/squirrel1.png",renderer,squirrelX,squirrelY);
        std::srand(squirrelX);
        squirrelX = std::rand() % 768;
        squirrelY = std::rand() % 576;
    }

    ParticleSystem sparkles(100);

    bool fullSCREEN;
    fullSCREEN = false;

    bool inventoryMenu;//display inventory if true
    inventoryMenu = false;

    // Initialize resource nodes
    std::vector<ResourceNode> resourceNodes;

    // Initialize tile map
    std::map<int, std::map<int, int>> tileMap;

    //initialize map
    std::vector<std::vector<int>> map(height, std::vector<int>(width, 0));

    int maxBarPixels = 59;
    int currentThirstBarPixels = 59;
    int currentHungerBarPixels = 59;
    int currentHealthBarPixels = 59;


    //SDL_Rect itemRects[inventory.size()];
    //Tooltip tooltip;
    /*
    std::vector<Button> buttons = {
        { {0, 0, 100, 30}, "Drop", dropItem },
        { {0, 0, 100, 30}, "Use", useItem },
        { {0, 0, 100, 30}, "Add to Hotbar", addToHotbar }
    };*/

    Button button7("images/buttons/drop.png","images/buttons/dropMO.png",renderer,100,30,8,8);
    Button button8("images/buttons/use.png","images/buttons/useMO.png",renderer,100,39,8,8);


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
    InventoryItem emptySlot(renderer,"emptySlot"," ","images/icons/emptySlot.png",InventoryItem::SOLID,false,false,-1);
    InventoryItem sword(renderer,"Sword", "A sharp blade used for combat.","images/icons/sword.png", InventoryItem::EQUIPPABLE, true, false, -1);
    InventoryItem potion(renderer,"Potion", "A healing potion for emergencies.","images/icons/potion.png", InventoryItem::SOLID, false, true);
    InventoryItem stone(renderer,"Stone","Smaller than a boulder, larger than a pebble.","images/stone1.png",InventoryItem::SOLID,false,false,-1,false,false,5,5,InventoryItem::FRESH);
    InventoryItem walnuts(renderer,"Walnuts","Shells like walnuts, taste like walnuts.","images/walnuts.png",InventoryItem::SOLID,false,true,-1,false,false,1,-1,InventoryItem::FRESH);
    InventoryItem branch(renderer,"Branch","A branch from a tree","images/branch1.png",InventoryItem::SOLID,false,false,-1,false,false,5,5,InventoryItem::FRESH);

    for(int i = 0; i < player.getMaxInventorySize(); i++)
    {
        player.addItem(emptySlot);
        //std::cout<<"emptySlot added to "<<player.getInventorySize();//<<" of "<<player.getMaxInventorySize();
    }
    for(int i = 0; i < player.getMaxHotBarSize(); i++)
    {
        player.addHotBarItem(emptySlot,i);
        //std::cout<<"emptySlot added to "<<player.getHotBar().size();//<<" of "<<player.getMaxInventorySize();
    }

    while (!quit) {
        endTick = SDL_GetTicks();

        float deltaTime = static_cast<float>(endTick - startTick);
        startTick = endTick;
        while (SDL_PollEvent(&e) != 0) {
            handleEvents(e,gameSTATE,player,quit,button1,button2,button3,button4,button5,button6,button7,button8,window,fullSCREEN,displayControls,cutSceneFinished,resourceNodes,gameState,tileMap,squirrels,bunnies,stone,branch,walnuts,emptySlot,inventoryMenu);

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

        //handleEvents,update,and render are where most of the action in the game occurs.
        //handle events takes input, update does the change in the game, render renders based on the changes.
        update(gameSTATE,splash,deltaTime,player,button1,button2,button3,button4,button5,button6,button7,button8,quit,height,width,map,tileMap,resourceNodes,renderer,displayControls,cutSceneFinished,myTimer,camera,gameState,squirrels,bunnies,sparkles,currentThirstBarPixels,currentHungerBarPixels,currentHealthBarPixels,inventoryMenu);
        player.updateItemsState();
        render(renderer,gameSTATE,splash,player,button1,button2,button3,button4,button5,button6,button7,button8,deltaTime,height,width,map,resourceNodes,displayControls,cutSceneFinished,myTimer,camera,squirrels,bunnies,sparkles,currentThirstBarPixels,currentHungerBarPixels,currentHealthBarPixels,inventoryMenu);

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
