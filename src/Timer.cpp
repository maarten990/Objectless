#include "Timer.h"
#include <SDL_timer.h>


Timer::Timer()
	: performance_frequency(SDL_GetPerformanceFrequency())
	, start_timestamp(SDL_GetPerformanceCounter())
{
}

double Timer::seconds_since_start() const
{
	return seconds_since(start_timestamp);
}

double Timer::seconds_since(uint64_t timestamp) const
{
	uint64_t elapsed = current_timestamp() - timestamp;
	return (double) elapsed / (double) performance_frequency;
}

uint64_t Timer::current_timestamp() const
{
	return SDL_GetPerformanceCounter();
}
