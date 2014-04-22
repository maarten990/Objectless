#include <SDL.h>
#include <vector>
#include <typeinfo>
#include "EventSystem.h"
#include "KeyBoardInputComponent.h"

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
			handle_keyevent(e.key.keysym.sym);
		}
	}
}

void EventSystem::handle_keyevent(SDL_Keycode k)
{
    // Check for each of the entities if the component can be downcast to
	// KeyBoardInputComponent. If so, try apply current keyboard event
	for (Component *component : _components) {
		if (typeid(*component) == typeid(KeyboardInput)) {
			KeyboardInput* kb = static_cast<KeyboardInput*>(component);

			// Check if key is in the dict. If so, apply function
			if(kb->keybinds.find(k) != kb->keybinds.end()) {
				kb->keybinds[k]();
			}
		}
	}
}
