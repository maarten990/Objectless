#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Texture;

class Texture
{
public:
	Texture(SDL_Renderer& renderer, const char* file_path);
	~Texture();


	unsigned int getWidth() const { return _width; }
	unsigned int getHeight() const { return _height; }
	SDL_Texture* getTexture() { return _texture; }
	const std::string& getName() const { return _name; }

private:
	SDL_Texture* _texture;
	unsigned int _width;
	unsigned int _height;
	std::string _name;
};