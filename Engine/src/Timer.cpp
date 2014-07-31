#include "Engine/Timer.h"
#if WIN32
#include <Windows.h>
#endif

Timer::Timer()
{
#if WIN32
	LARGE_INTEGER i;
	QueryPerformanceFrequency(&i);
	frequency = i.QuadPart;
#endif
	start_point = current_timestamp();
}

double Timer::seconds_since_start() const
{
	return seconds_since(start_point);
}

double Timer::seconds_since(const Timestamp& timestamp) const
{
	Timer::Timestamp now = current_timestamp();
	return calculateDifference(timestamp, now);
}

Timer::Timestamp Timer::current_timestamp() const
{
#if WIN32
	LARGE_INTEGER i;
	QueryPerformanceCounter(&i);
	return i.QuadPart;
#elif __linux__
	timespec timestamp;
	clock_gettime(CLOCK_MONOTONIC, &timestamp);
	return timestamp;
#else
#error "Unknown platform"
#endif
}

double Timer::calculateDifference(const Timer::Timestamp& start, const Timer::Timestamp& end) const
{
#if WIN32
	int64_t difference = end - start;
	return (double)difference / (double)frequency;
#elif __linux__
	const int64_t secs_to_nanosecs = 1000 * 1000 * 1000;
	const double nanosecs_to_secs = 1.0 / (double)secs_to_nanosecs;

	int64_t start_nanosecs = start.tv_sec * secs_to_nanosecs + start.tv_nsec;
	int64_t end_nanosecs = end.tv_sec * secs_to_nanosecs + end.tv_nsec;
	return (end_nanosecs - start_nanosecs) * nanosecs_to_secs;
#else
#error "Unknown platform"
#endif
}
