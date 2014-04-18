#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H value

#include "System.h"

class EventSystem : public System
{
public:
	EventSystem(bool *running);
	~EventSystem();
	void step(unsigned int dt);
	bool *running;
};

#endif
