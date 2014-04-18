#include <SDL.h>
#include <iostream>
#include <vector>
#include "GraphicsSystem.h"
#include "EventSystem.h"
#include "SystemManager.h"

int main()
{
	// initialize sdl
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER |
				SDL_INIT_AUDIO) < 0) {
		std::cout << "Could not initialize SDL. SDL error: " <<
			SDL_GetError() << std::endl;
		return -1;
	}

	// create systems
	// TODO: this bool pointer that gets passed around to two classes sucks
	bool running = true;

	SystemManager manager(60, &running);

	manager.add( new GraphicsSystem() );
	manager.add( new EventSystem(&running) );

	manager.loop();

	return 0;
}
