#include "Enemy.h"
#include "texture.h"  // Adjust this to your texture manager header
#include <cstdlib>
#include <ctime>
#include <iostream>

Enemy::Enemy(const char* texturePath, SDL_Renderer* renderer, int x, int y)
    : xPos(x), yPos(y), xVel(0.0f), yVel(0.0f), collisionBox(x, y, 16, 16) {  // Initialize collision box
    texture = IMG_LoadTexture(renderer, texturePath);  // Load texture
    std::srand(std::time(0));  // Seed for random number generation

    walkTimer = 0.0f;
    walkDuration = std::rand() % 2 + 1;  // Random duration between 1 and 2 seconds

    // Initialize srcRect and destRect
    srcRect = { 0, 0, 16, 16 };  // Assuming your texture is 16x16 pixels
    destRect = { xPos, yPos, 16, 16 };  // Destination rectangle same size as texture
}

Enemy::~Enemy() {
    SDL_DestroyTexture(texture);
    CollisionBox dead(0,0,0,0);
    collisionBox = dead;
}

void Enemy::update(float deltaTime, int x, int y ) {

    walkTimer += deltaTime;
    if (walkTimer >= walkDuration) {

        if(aggroRange(x,y))
        {
            //avoidPlayer(x,y);
            seekPlayer(x,y);
            std::cout<<"\n seeking player";
        }
        else
        {
            randomWalk();
            std::cout<<"\n random walk";
        }
        walkTimer = 0.0f;
        walkDuration = std::rand() % 2 + 1;  // Reset walk duration
    }
    collisionBox.updatePosition(xPos, yPos);  // Update collision box position

    // Update destRect position
    destRect.x = xPos;
    destRect.y = yPos;


}

void Enemy::render(SDL_Renderer* renderer, const SDL_Rect& cameraRect) {
    SDL_Rect renderQuad = { xPos - cameraRect.x, yPos - cameraRect.y, destRect.w, destRect.h };
    SDL_RenderCopy(renderer, texture, &srcRect, &renderQuad);
}

void Enemy::randomWalk() {
    int direction = std::rand() % 4;  // Random direction (0: up, 1: down, 2: left, 3: right)
    int moveDistance = 5;  // Distance to move in each step
    switch (direction) {
        case 0: yPos -= moveDistance; break;  // Move up
        case 1: yPos += moveDistance; break;  // Move down
        case 2: xPos -= moveDistance; break;  // Move left
        case 3: xPos += moveDistance; break;  // Move right
    }
}

bool Enemy::aggroRange(int x, int y)
{
    int distance = 100;
    bool inRange = false;
    bool xAxisInRange = false;
    bool yAxisInRange = false;
    if(x>xPos)
    {
        if((x-xPos)< distance)
        {
            xAxisInRange = true;
        }
    }
    else
    {
        if((xPos-x)<distance)
        {
            xAxisInRange = true;
        }
    }
    if(y>yPos)
    {
        if((y-yPos) < distance)
        {
            yAxisInRange = true;
        }
    }
    else
    {
        if((yPos-y)<distance)
        {
            yAxisInRange = true;
        }
    }
    if(xAxisInRange && yAxisInRange)
    {
        inRange=true;
    }
    return inRange;
}

void Enemy::seekPlayer(int x, int y){
    //std::cout<<"\n xPos - x = "<<(xPos - x);
    int distance = 100;//aggro range
    if(x>xPos)//player is right of enemy
    {
        if(y>yPos)//player is below enemy
        {
            if( (y-yPos) < distance )//if distance below enemy is less than 30
            {
                if(  (x - xPos) < distance  )//if player is less than 30 pixels right of enemy
                {
                    xPos+=5;//move enemy 5 pixels right.
                }
            }
        }
        else//player is above enemy
        {
            if((yPos - y) < distance)//if distance above enemy is less than 30
            {
                if((x-xPos) < distance)//player is less than 30 pix right of enemy
                {
                    xPos+=5;//move enemy 5 pixels right
                }
            }
        }
    }
    else//player is left of enemy
    {
        if(y>yPos)//player is below enemy
        {
            if( (y-yPos) < distance) // distance between player and enemy is less than 30 pixels on y axis
            {
                if((xPos - x) < distance)//enemy is less than 30 pixels right of player
                {
                    xPos-=5;
                }
            }
        }
        else//player is above enemy
        {
            if((yPos-y) < distance)//player is less than 30 px above enemy
            {
                if((xPos-x)<distance)//enemy is less than 30 pixels right of player
                {
                    xPos-=5;
                }
            }
        }
    }
    if(y>yPos)//player is below enemy
    {
        if(x>xPos)//if player is right of enemy
        {
            if((x-xPos)<distance)//player is less than 30 px right of enemy
            {
                if((y-yPos)<distance)//player is less than 30 px below enemy
                {
                    yPos+=5;//move enemy 5 px down
                }
            }
        }
        else//player is left of enemy
        {
            if((xPos - x) < distance ) // less than 30 px left of enemy
            {
                if((y-yPos)<distance) // less than 30 px below enemy
                {
                    yPos+=5;//move enemy down 5 px
                }
            }
        }
    }
    else//player is above enemy
    {
        if(x>xPos)//player is right of enemy
        {
            if((x-xPos)<distance)//player is less than 30 px right of enemy
            {
                if((yPos-y) < distance)//player is less than 30 px above enemy
                {
                    yPos-=5;//move enemy up 5 px
                }
            }
        }
        else//player is left of enemy
        {
            if((xPos-x)<distance)//player is less than 30 px left of enemy
            {
                if((yPos-y) < distance)//player is less than 30 px above enemy
                {
                    yPos-=5;//move enemy up 5 px
                }
            }
        }
    }
}

void Enemy::avoidPlayer(int x, int y){
    //std::cout<<"\n xPos - x = "<<(xPos - x);
    int distance = 100;
    if(x>xPos)//player is right of enemy
    {
        if(y>yPos)//player is below enemy
        {
            if( (y-yPos) < distance )//if distance below enemy is less than 30
            {
                if(  (x - xPos) < distance  )//if player is less than 30 pixels right of enemy
                {
                    xPos-=5;//move enemy 5 pixels left.
                }
            }
        }
        else//player is above enemy
        {
            if((yPos - y) < distance)//if distance above enemy is less than 30
            {
                if((x-xPos) < distance)//player is less than 30 pix right of enemy
                {
                    xPos-=5;//move enemy 5 pixels left
                }
            }
        }
    }
    else//player is left of enemy
    {
        if(y>yPos)//player is below enemy
        {
            if( (y-yPos) < distance) // distance between player and enemy is less than 30 pixels on y axis
            {
                if((xPos - x) < distance)//enemy is less than 30 pixels right of player
                {
                    xPos+=5;
                }
            }
        }
        else//player is above enemy
        {
            if((yPos-y) < distance)//player is less than 30 px above enemy
            {
                if((xPos-x)<distance)//enemy is less than 30 pixels right of player
                {
                    xPos+=5;
                }
            }
        }
    }
    if(y>yPos)//player is below enemy
    {
        if(x>xPos)//if player is right of enemy
        {
            if((x-xPos)<distance)//player is less than 30 px right of enemy
            {
                if((y-yPos)<distance)//player is less than 30 px below enemy
                {
                    yPos-=5;//move enemy 5 px up
                }
            }
        }
        else//player is left of enemy
        {
            if((xPos - x) < distance ) // less than 30 px left of enemy
            {
                if((y-yPos)<distance) // less than 30 px below enemy
                {
                    yPos-=5;//move enemy up 5 px
                }
            }
        }
    }
    else//player is above enemy
    {
        if(x>xPos)//player is right of enemy
        {
            if((x-xPos)<distance)//player is less than 30 px right of enemy
            {
                if((yPos-y) < distance)//player is less than 30 px above enemy
                {
                    yPos+=5;//move enemy down 5 px
                }
            }
        }
        else//player is left of enemy
        {
            if((xPos-x)<distance)//player is less than 30 px left of enemy
            {
                if((yPos-y) < distance)//player is less than 30 px above enemy
                {
                    yPos+=5;//move enemy down 5 px
                }
            }
        }
    }
}

void Enemy::dead(){

}

const CollisionBox& Enemy::getCollisionBox() const {
    return collisionBox;
}
