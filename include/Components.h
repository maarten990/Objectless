#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include "SDL.h"
#include "Engine/Component.h"

#include <Box2D/Common/b2Math.h>

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
		/* TODO: maybe destroy the texture in a more appropriate place */
	    SDL_DestroyTexture(texture);
	}


	SDL_Texture *texture = nullptr;
	int width = 0;
	int height = 0;
};

struct Transform : Component {
	Transform()
	{
		position.SetZero();
		rotation.SetIdentity();
	}

	b2Vec2 position;
	b2Rot rotation;

	float x;
	float y;
};

struct Movement : Component {
	float x_multiplier;
	float y_multiplier;
	float x_speed;
	float y_speed;
};

#endif
