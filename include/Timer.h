#pragma once

#include <stdint.h>
#include <chrono>

class Timer
{
public:
	Timer();

	// Returns seconds since this instance was created.
	double seconds_since_start() const;

	// Returns seconds since the specified timestamp.
	double seconds_since(uint64_t timestamp) const;

	// Returns the current timestamp.
	uint64_t current_timestamp() const;

private:
	std::chrono::high_resolution_clock clock;
	std::chrono::system_clock::time_point start_point;
};
