#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H value

#include <SDL.h>

class GraphicsSystem
{
public:
	GraphicsSystem();
	~GraphicsSystem();

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;
};

#endif
