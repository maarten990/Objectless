#pragma once

#include "System.h"
#include <vector>
#include <memory>
#include "EntityManager.h"

class SystemManager {
public:
	SystemManager(int max_fps, shared_ptr<bool> running);
	~SystemManager();

	/* Add a system.
	 * The argument should be a pointer to a heap-allocated system.
	 * The pointer will be freed in SystemManager's destructor. */
	void add(shared_ptr<System> system);

	/* Enter a loop which calls each system's step function at a fixed
	 * interval.
	 * Halts when the _running member becomes false.
	 */
	void loop();

	/* Holds the average fps over the class' lifespan */
	float avg_fps;

private:
	std::vector<shared_ptr<System> > _systems;
	shared_ptr<bool> _running;

	// The minimum frame duration needed to not exceed max fps.
	float _max_fps_frame_duration;
};
