#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H value

#include <SDL.h>
#include <memory>
#include "Engine/System.h"

class EventSystem : public System
{
public:
	EventSystem(shared_ptr<bool> running, shared_ptr<EntityManager> em);
	~EventSystem();
	void step(float delta_time);
	shared_ptr<bool> running;
	void handle_keyevent(SDL_Keycode k);

private:
	shared_ptr<EntityManager> _entitymanager;
};

#endif
