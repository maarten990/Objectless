#ifndef COMPONENT_H
#define COMPONENT_H value

#include "GraphicsSystem.h"
#include <SDL.h>
#include <map>
#include <string>
#include <functional>
#include <typeindex>
#include <typeinfo>

using namespace std;

class Component
{
public:
    static map<int, function<Component*(void*)> > factory;
    type_index get_id() {
        return typeid(*this);
    }
    virtual ~Component() {};
};

struct Graphics : Component {
	Graphics(SDL_Texture *texture)
       	{
	    this->texture = texture;
	};
	~Graphics() 
	{
	    SDL_DestroyTexture(texture);
	};
	SDL_Texture *texture;
};

struct Position : Component {
	float x;
	float y;
	float rotation;
};

#endif
