#include "Engine/Graphics/Texture.h"
#include <SDL_image.h>
#include "Assert2.h"

Texture::Texture(SDL_Renderer& renderer, const char* file_path)
	: _name(file_path)
{
	SDL_Surface* loadedSurface = IMG_Load(file_path);
	const char* errorMessage = "Failed to load image at path '%s'. SDL Error: %s";
	assert2(loadedSurface != nullptr, errorMessage, file_path, SDL_GetError());

	_width = loadedSurface->w;
	_height = loadedSurface->h;

	_texture = SDL_CreateTextureFromSurface(&renderer, loadedSurface);
	assert2(_texture != nullptr, errorMessage, file_path, SDL_GetError());

	SDL_FreeSurface(loadedSurface);

	printf("Texture '%s' loaded successfully.\n", file_path);
}

Texture::~Texture()
{
	SDL_DestroyTexture(_texture);
}

