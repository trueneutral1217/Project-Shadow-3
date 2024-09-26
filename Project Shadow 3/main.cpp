#include <SDL.h>
#include <stdio.h>
#include "texture.h"

SDL_Renderer* renderer = NULL;
texture demo;

int main( int argc, char* args[] )
{
    //The window we'll be rendering to
    SDL_Window* window = NULL;

    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //flags for window
        Uint32 flags = SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
        //Create window
        window = SDL_CreateWindow("Project Shadow 3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 192, flags);
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
        if( renderer == NULL )
        {
            printf( "\n Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );

            //Fill the surface white
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

            //Update the surface
            SDL_UpdateWindowSurface( window );

            if(!demo.loadFromFile("images/demoScene.png",renderer))
            {
                printf("\n demo image failed to load! SDL Error: %s \n",SDL_GetError());
            }

            //Hack to get window to stay up
            SDL_Event e;
            bool quit = false;
            while( quit == false )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }

                //Clear screen
                SDL_RenderClear( renderer );
                SDL_Rect bounds;
                SDL_GetDisplayBounds(0,&bounds);

                //set viewport in center of screen
				SDL_Rect viewport;
				viewport.x = (bounds.w - SCREEN_WIDTH)/2;
				viewport.y = (bounds.h - SCREEN_HEIGHT)/2;
				viewport.w = SCREEN_WIDTH;
				viewport.h = SCREEN_HEIGHT;
				SDL_RenderSetViewport( renderer, &viewport );

                demo.render(0,0,NULL,0.0,NULL,SDL_FLIP_NONE,renderer);

                //Update screen
                SDL_RenderPresent( renderer );
            }

        }
    }

    demo.free();

    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    //Destroy window
    SDL_DestroyWindow( window );
    window = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();

    return 0;
}
