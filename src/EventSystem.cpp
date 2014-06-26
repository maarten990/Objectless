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

EventSystem::~EventSystem ()
{
}


void EventSystem::step(float /*delta_time*/)
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
	for (unsigned int entity : _entities) {

        Component* component = _entitymanager->get<KeyboardInput>(entity);
		KeyboardInput* kb = static_cast<KeyboardInput*>(component);

        // Check if key is in the dict. If so, apply function
        if(kb->keybinds.find(k) != kb->keybinds.end()) {
            kb->keybinds[k]();
        }
    
    }
}
