#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H value

<<<<<<< HEAD
#include <SDL.h>
=======
#include "SDL.h"
#include "System.h"
>>>>>>> 8db2c7206a192b48b3fbab697513d206cf543124

class GraphicsSystem : public System
{
public:
	GraphicsSystem();
	~GraphicsSystem();
	void step(unsigned int dt);

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;
};

#endif
