#include <SDL.h>
#include <SDL_image.h>
#include "GraphicsSystem.h"
#include "EntityManager.h"
#include "Component.h"

Uint32 graphics_callback(Uint32 interval, void *graphics)
{
	GraphicsSystem *system = static_cast<GraphicsSystem*>(graphics);

	return interval;
}

GraphicsSystem::GraphicsSystem()
{
	_window = SDL_CreateWindow("Objectless", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED |
		   	SDL_RENDERER_PRESENTVSYNC);

    // Initialize the flags for image lib
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    
        SDL_GetError();
        exit(0);
    }

	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(_renderer);
	SDL_AddTimer(100, graphics_callback, this);
}



GraphicsSystem::~GraphicsSystem()
{
	SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);

}


// Load a texture from file
SDL_Texture* GraphicsSystem::loadTexture(string path)
{
    SDL_Texture* loadedTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface) {
        loadedTexture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
        if(!loadedTexture) {
            cout<<"Error: SDL_Texture* could not be created from image at " << path <<". Exiting..."<<endl;
            SDL_GetError();
            exit(0);
        }
        else
        {
        
            cout<<"Texture loaded succesfully"<<endl;
        }
        SDL_FreeSurface( loadedSurface );
    }
    else {
        cout<<"Error: Image at " <<path<< " could not be loaded as SDL_Surface*. Exiting..."<<endl;
          SDL_GetError();
        exit(0);
    }
    return loadedTexture;
}

void GraphicsSystem::step(unsigned int dt, EntityManager* em)
{

    /* TODO: add a correspondence between components with the same entity, use
     * Position */
    for (unsigned int id: _entities) {
            // Retrieve the Graphics and Position component
            Graphics *g = em->get<Graphics>(id);
            Position *p = em->get<Position>(id);
            SDL_RenderCopy(_renderer, g->texture, NULL, NULL);  
            SDL_GetError();
    }

    SDL_RenderPresent(_renderer);
}
