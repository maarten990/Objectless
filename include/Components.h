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
	}

	static type_index id() {
	    return typeid(Graphics);
	}

	SDL_Texture *texture;
	int width;
	int height;
};

struct Position : Component {
	float x;
	float y;
	float rotation;

	static type_index id() {
	    return typeid(Position);
	}
};

#endif
