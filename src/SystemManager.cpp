#include <SDL.h>
#include "SystemManager.h"

SystemManager::SystemManager(int fps, bool *running)
{
	_running = running;
	_frametime = (1.0 / fps) * 1000;
}

SystemManager::~SystemManager()
{
}

void SystemManager::add(System *system)
{
	_systems.push_back(system);
}

/* Loops until _running becomes false.
 *
 * If the time spent in a single iteration is lower than the frame duration
 * specified by the framerate, the function idles for the remaining time.
 *
 * The average framerate is tracked and written to the avg_fps member.
 */
void SystemManager::loop()
{
	int frames = 0;
	const Uint32 initial_ticks = SDL_GetTicks();
	Uint32 ticks = 0;
	Uint32 prev_ticks = SDL_GetTicks();
	Uint32 duration;
	Uint32 dt;
	float diff;

	while (*_running) {
		ticks = SDL_GetTicks();
		frames += 1;

		// calculate average fps
		diff = ticks - initial_ticks;
		avg_fps = (frames * 1000) / diff;

		// call systems
		for (System *system : _systems) {
			system->step(ticks - prev_ticks);
			prev_ticks = ticks;
		}

		// limit framerate
		duration = SDL_GetTicks() - ticks;
		if (duration < _frametime) {
			SDL_Delay(_frametime - duration);
		}
	}
}
