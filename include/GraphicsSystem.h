#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include <SDL.h>
#include <iostream>

#include "Engine/System.h"

class GraphicsSystem : public System
{
public:
	GraphicsSystem();
	~GraphicsSystem();
	SDL_Texture* loadTexture(std::string path);
	void step(unsigned int dt, EntityManager* em);

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;
};

#endif
