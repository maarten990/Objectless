#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include "SDL.h"
#include "Engine/Component.h"

struct Graphics : Component {
	Graphics()
	{
	}

	Graphics(SDL_Texture *texture)
       	{
	    this->texture = texture;
	}

	~Graphics() 
	{
	    SDL_DestroyTexture(texture);
	}


	SDL_Texture *texture;
	int width;
	int height;
};

struct Position : Component {
	float x;
	float y;
	float rotation;
};

#endif
