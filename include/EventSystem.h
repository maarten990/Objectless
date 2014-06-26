#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H value

#include <SDL.h>
#include "Engine/System.h"

class EventSystem : public System
{
public:
	EventSystem(bool *running, EntityManager *em);
	~EventSystem();
	void step(float delta_time);
	bool *running;
	void handle_keyevent(SDL_Keycode k);

private:
	EntityManager *_entitymanager;
};

#endif
