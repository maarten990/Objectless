#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H value

#include <SDL.h>
#include "System.h"

class GraphicsSystem : public System
{
public:
	GraphicsSystem();
	~GraphicsSystem();
	void step(unsigned int dt);

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;
};

#endif
