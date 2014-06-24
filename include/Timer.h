#pragma once

#include <stdint.h>

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
	uint64_t performance_frequency;
	uint64_t start_timestamp;
};