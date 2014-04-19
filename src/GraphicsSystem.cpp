#include <SDL.h>
#include <SDL_image.h>
#include "GraphicsSystem.h"
#include "Entity.h"
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
	cout<<"set window"<<_window<<endl;
	cout<<"set renderer"<<_renderer<<endl;
	cout<<SDL_GetError()<<endl;

	SDL_AddTimer(100, graphics_callback, this);
}

GraphicsSystem::~GraphicsSystem()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
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
            exit(0);
        }
    }
    else {
        cout<<"Error: Image at " <<path<< " could not be loaded as SDL_Surface*. Exiting..."<<endl;
        exit(0);
    }
    SDL_FreeSurface( loadedSurface );
    return loadedTexture;
}


void GraphicsSystem::step(unsigned int dt)
{
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(_renderer);
	// Loop throug entities. retrieve position adn graphics component and
	// draw texture to screen
	for (int i = 0; i < entities.size(); i ++) {
	    for (int j = 0; j < entities.size(); j ++) {
		if (Graphics* g = dynamic_cast<Graphics*>(entities[i][j])) {
		    SDL_RenderCopy(_renderer, g->texture, NULL, NULL);  
		}
	    }
	}
}
