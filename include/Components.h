#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include "SDL.h"
#include "Engine/Component.h"

#include <Box2D/Common/b2Math.h>

struct Graphics : Component {

	SDL_Texture *texture = nullptr;
	int width = 0;
	int height = 0;
};

struct Transform : Component {

	float x;
	float y;
	b2Vec2 position = b2Vec2(0.0f, 0.0f);
	b2Rot rotation = b2Rot(0.0f);
};

#endif
