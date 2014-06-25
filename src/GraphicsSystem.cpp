#include <SDL.h>
#include <SDL_image.h>

#include "GraphicsSystem.h"
#include "Components.h"
#include "Engine/EntityManager.h"
#include "Assert.h"


GraphicsSystem::GraphicsSystem()
{
	_window = SDL_CreateWindow("Objectless", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED |
		   	SDL_RENDERER_PRESENTVSYNC);

    // Initialize the flags for image lib
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
			assert2(false, "IMG_Init failed!! %s", SDL_GetError());
			exit(0);
    }

	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(_renderer);
}



GraphicsSystem::~GraphicsSystem()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
}


// Load a texture from file
SDL_Texture* GraphicsSystem::loadTexture(const std::string& path)
{
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	const char* errorMessage = "Failed to load image at path '%s'. SDL Error: %s";
	assert2(loadedSurface != nullptr, errorMessage, path.c_str(), SDL_GetError());

	SDL_Texture* loadedTexture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
	assert2(loadedTexture != nullptr, errorMessage, path.c_str(), SDL_GetError());

	SDL_FreeSurface(loadedSurface);

	printf("Texture '%s' loaded successfully.", path.c_str());
	return loadedTexture;
}

void GraphicsSystem::step(float delta_time, EntityManager* em)
{
    SDL_RenderClear(_renderer);

    for (unsigned int id: _entities) {
        Graphics *g = em->get<Graphics>(id);
        Position *p = em->get<Position>(id);

        /* create destination rect using the desired position 
         * and the texture's dimensions */
        SDL_Rect dest;
        dest.x = (int)p->x;
        dest.y = (int)p->y;
        dest.w = g->width;
        dest.h = g->height;

        SDL_RenderCopy(_renderer, g->texture, nullptr, &dest);
        SDL_GetError();
    }

    SDL_RenderPresent(_renderer);
}
