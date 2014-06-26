#include "Timer.h"


Timer::Timer()
	: start_point(clock.now())
{
}

double Timer::seconds_since_start() const
{
	return seconds_since(0);
}

double Timer::seconds_since(uint64_t timestamp) const
{
	const long long nano_secs = current_timestamp() - timestamp;
	const double seconds = (double)nano_secs / std::chrono::nanoseconds::period::den;
	return seconds;
}

uint64_t Timer::current_timestamp() const
{
	return std::chrono::nanoseconds(clock.now() - start_point).count();
}
