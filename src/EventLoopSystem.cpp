#include <SDL.h>
#include "EventLoopSystem.h"

EventLoopSystem::EventLoopSystem()
{
}

EventLoopSystem::~EventLoopSystem ()
{
}

void EventLoopSystem::loop()
{
	bool quit;
	SDL_Event e;

	while( !quit ) {
		while( SDL_PollEvent( &e ) != 0 ) {
			if( e.type == SDL_QUIT )
				quit = true; 
		}
	}
}
