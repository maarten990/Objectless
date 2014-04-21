#ifndef COMPONENT_H
#define COMPONENT_H value

#include <SDL.h>
#include "GraphicsSystem.h"
#include <iostream>

class Component
{
public:
    virtual ~Component() {};
};

struct Graphics : Component {
	Graphics(SDL_Texture *texture)
       	{
	    this->texture = texture;
	};
	~Graphics() 
	{
	    SDL_DestroyTexture(texture);
	};
	SDL_Texture *texture;
};

struct Position : Component {
	float x;
	float y;
	float rotation;
};

#endif
