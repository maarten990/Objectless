#include <SDL.h>
#include "GraphicsSystem.h"
#include "Entity.h"
#include <iostream>

Uint32 graphics_callback(Uint32 interval, void *graphics)
{
	GraphicsSystem *system = static_cast<GraphicsSystem*>(graphics);

	return interval;
}

GraphicsSystem::GraphicsSystem()
{
	_window = SDL_CreateWindow("Objectless", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	SDL_AddTimer(100, graphics_callback, this);
}

GraphicsSystem::~GraphicsSystem()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
}
