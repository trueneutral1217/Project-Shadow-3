#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "string"
//#include "string.h"
#include "stdio.h"
#include "sstream"
#include "SDL_image.h"
#include <cmath>
#include <stddef.h>
#include <iostream>
//#include <istream>

#include <assert.h>
#include <vector>
#include "cstdlib"
//#include "timer.h"

const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 192;


class texture
{
	public:
		//Initializes variables
		texture();

		//Deallocates memory
		~texture();

        //creates individual image as texture
        SDL_Texture* loadTexture( std::string path,SDL_Renderer* renderer);

		//bool initTexture(int SCREEN_WIDTH,int SCREEN_HEIGHT, SDL_Window* window,SDL_Renderer* renderer);

		//Loads image at specified path, if texture already exists
		bool loadFromFile( std::string fileLocation, SDL_Renderer* renderer);

		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string text, SDL_Color color,TTF_Font* font,SDL_Renderer* renderer );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE,SDL_Renderer* renderer=NULL );
        //currently not ready
		//void timedRender(int ticks, int x, int y, texture& tempTexture, SDL_Renderer* renderer=NULL);

		//Gets image dimensions
		int getWidth();
		int getHeight();



	private:
		//The actual hardware texture
		SDL_Texture* Texture;

		//Image dimensions
		int width;
		int height;
};

#endif // TEXTURE_H
