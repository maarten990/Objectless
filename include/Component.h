#ifndef COMPONENT_H
#define COMPONENT_H value

#include <SDL.h>

class Component
{
	public:
};

struct Graphics {
	Graphics() {};
	~Graphics() {};
	SDL_Texture *texture;
};

struct Position {
	float x;
	float y;
	float rotation;
};

#endif
