#pragma once

#include <stdint.h>

#if __linux__
#include <time.h>
#endif

class Timer
{
public:
#if WIN32
	typedef int64_t Timestamp;
#elif __linux__
	typedef timespec Timestamp;
#else
#error "Unknown platform"
#endif

	Timer();

	// Returns seconds since this instance was created.
	double seconds_since_start() const;

	// Returns seconds since the specified timestamp.
	double seconds_since(const Timestamp& timestamp) const;

	// Returns the current timestamp.
	Timestamp current_timestamp() const;

private:
	//Calculate the time between two time stamps, in seconds.
	double calculateDifference(const Timestamp& start, const Timestamp& end) const;

#if WIN32
	int64_t frequency;
	int64_t start_point;
#elif __linux__
	timespec start_point;
#else
#error "Unknown platform"
#endif
};
