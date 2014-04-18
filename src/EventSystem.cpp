#include <SDL.h>
#include <vector>
#include "EventSystem.h"
#include "KeyBoardInputSystem.h"

EventSystem::EventSystem(bool *running)
{
	this->running = running;
}

EventSystem::~EventSystem ()
{
}

void EventSystem::step(unsigned int dt)
{
	SDL_Event e;

	while( SDL_PollEvent( &e ) != 0 ) {
		if( e.type == SDL_QUIT )
			*running = false;
        if(e.type == SDL_KEYDOWN) {
            keyinput.handleKeyEvent(e.key.keysym.sym);
        }
	}
}
