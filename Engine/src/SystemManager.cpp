#include <SDL.h>
#include "Engine/SystemManager.h"
#include "Engine/EntityManager.h"
#include "Timer.h"



SystemManager::SystemManager(int max_fps, bool *running)
{
	_running = running;
	_max_fps_frame_duration = (1.0f / max_fps);
	avg_fps = 60.0f;
}

SystemManager::~SystemManager()
{
	for (System *system : _systems)
		delete system;
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
	uint32_t current_frame_count = 0;

	Timer timer;
	double previous_frame_duration = 0.0166667;

	while (*_running) {
		++current_frame_count;
		const uint64_t frame_start_timestamp = timer.current_timestamp();

		// call systems
		for (System *system : _systems) {
			system->step((float)previous_frame_duration);
		}

		previous_frame_duration = timer.seconds_since(frame_start_timestamp);
		//Note: This fps does not include the delay used to stay below max fps.
		double fps = 1.0f / previous_frame_duration;

		// calculate moving average fps
		const float alpha = 0.01f;
		avg_fps = (float)((alpha * fps) + (1.0 - alpha) * avg_fps);

		// limit framerate
		if (previous_frame_duration < _max_fps_frame_duration) {
			//Remaining time this frame must take to process in order to not exceed target frame rate.
			double remaining_frame_duration_milliseconds = (_max_fps_frame_duration - previous_frame_duration) * 1000.0;
			SDL_Delay((uint32_t)remaining_frame_duration_milliseconds);
		}
	}
}
