#include <SDL.h>
#include <SDL_image.h>
#include <inttypes.h>
#include <memory>

#include "GraphicsSystem.h"
#include "Components.h"
#include "Engine/EntityManager.h"
#include "Assert.h"


GraphicsSystem::GraphicsSystem(shared_ptr<EntityManager> em)
{
	_window = SDL_CreateWindow("Objectless", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED |
		   	SDL_RENDERER_PRESENTVSYNC);

	_entitymanager = em;

    // Initialize the flags for image lib
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
			assert2(false, "IMG_Init failed!! %s", SDL_GetError());
			exit(0);
    }
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

	printf("Texture '%s' loaded successfully.\n", path.c_str());
	return loadedTexture;
}

void GraphicsSystem::step(float /*delta_time*/)
{
	SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x5B, 0xFF);
    SDL_RenderClear(_renderer);

    for (unsigned int id: _entities) {
        shared_ptr<Graphics> g = _entitymanager->get<Graphics>(id);
        shared_ptr<Position> p = _entitymanager->get<Position>(id);

        /* create destination rect using the desired position 
         * and the texture's dimensions */
        SDL_Rect dest;
        dest.x = (int)p->x;
        dest.y = (int)p->y;
        dest.w = g->width;
        dest.h = g->height;

        SDL_RenderCopy(_renderer, g->texture, nullptr, &dest);
    }

		for (auto post_render_callback : _post_render_callbacks)
		{
			post_render_callback.function();
		}

    SDL_RenderPresent(_renderer);
}

uint64_t GraphicsSystem::registerPostRenderCallback(const std::function<void()> callback)
{
	const uint64_t callback_id = _num_post_render_callbacks_added++;
	_post_render_callbacks.push_back(Callback{ callback_id, callback });
	return callback_id;
}

void GraphicsSystem::unregisterPostRenderCallback(uint64_t callback_id)
{
	const auto itr = std::find_if(std::begin(_post_render_callbacks), std::end(_post_render_callbacks),
		[callback_id](const Callback& callback) {return callback.id == callback_id; });

	assert2(itr != std::end(_post_render_callbacks),
		"No post render callback with ID %" PRIu64 " exists.", callback_id);

	_post_render_callbacks.erase(itr);
}
