#include <SDL.h>
#include <iostream>
#include <vector>
#include "GraphicsSystem.h"
#include "EventSystem.h"
#include "EntityManager.h"

const int FPS = 60;
const float FRAMEDURATION = (1.0 / 60) * 1000; // frame duration in ms

int main()
{
	// initialize sdl
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER |
				SDL_INIT_AUDIO) < 0) {
		std::cout << "Could not initialize SDL. SDL error: " <<
			SDL_GetError() << std::endl;
		return -1;
	}

	// create systems
	bool running = true;

	std::vector<System*> systems;

	EventSystem loop(&running);
	GraphicsSystem graphics;

    EntityManager entities();

	systems.push_back(&loop);
	systems.push_back(&graphics);

	// main loop
	int frames = 0;
	float avgfps = 0;
	const Uint32 initial_ticks = SDL_GetTicks();
	Uint32 start;
	Uint32 duration;

	while (running) {
		start = SDL_GetTicks();
		frames += 1;

		float diff = SDL_GetTicks() - initial_ticks;
		avgfps = (frames * 1000) / diff;
		std::cout << "Average fps: " << avgfps << std::endl;

		for (System *system : systems) {
			system->step(0xDEADBEEF);
		}

		// wait to limit framerate
		duration = SDL_GetTicks() - start;
		if (duration < FRAMEDURATION) {
			SDL_Delay(FRAMEDURATION - duration);
		}
	}

	return 0;
}
