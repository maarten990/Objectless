#include <SDL.h>
#include <vector>
#include <typeinfo>

#include "Engine/EntityManager.h"
#include "EventSystem.h"
#include "KeyBoardInputComponent.h"

EventSystem::EventSystem(bool *running, EntityManager *em)
{
	this->running = running;
	_entitymanager = em;
}

EventSystem::~EventSystem()
{
}

void EventSystem::step(float /*delta_time*/)
{
	SDL_Event e;

	while( SDL_PollEvent( &e ) != 0 ) {
		switch (e.type) {
			case SDL_QUIT:
				*running = false;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				handle_keyevent(e.key.keysym.sym, e.type);
				break;
		}
	}
}

void EventSystem::handle_keyevent(SDL_Keycode k, Uint32 type)
{
	// Check for each of the entities if the component can be downcast to
	// KeyBoardInputComponent. If so, try apply current keyboard event
	for (unsigned int entity : _entities) {
		KeyboardInput* kb = _entitymanager->get_component<KeyboardInput>(entity);

        // Check if key is in the dict. If so, apply function
        if(kb->keybinds.find(k) != kb->keybinds.end())
			if (kb->keybinds[k].find(type) != kb->keybinds[k].end())
				kb->keybinds[k][type]();
    }
}
