#include <SDL.h>
#include <iostream>
#include "GraphicsSystem.h"
#include "EventLoopSystem.h"

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER |
				SDL_INIT_AUDIO) < 0) {
		std::cout << "Could not initialize SDL. SDL error: " <<
			SDL_GetError() << std::endl;
		return -1;
	}

	GraphicsSystem sdl;
	EventLoopSystem loop;

	loop.loop();

	return 0;
}
