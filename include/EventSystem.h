#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H value

#include <SDL.h>
#include "Engine/System.h"

class EventSystem : public System
{
public:
	EventSystem(bool *running);
	~EventSystem();
	void step(unsigned int dt, EntityManager* em);
	bool *running;
	void handle_keyevent(SDL_Keycode k, EntityManager* em);
};

#endif
