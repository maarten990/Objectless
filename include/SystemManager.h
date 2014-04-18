#include "System.h"
#include <vector>

class SystemManager {
public:
	SystemManager(int fps, bool *running);
	~SystemManager();

	/* add a system */
	void add(System *system);

	/* Enter a loop which calls each system's step function at a fixed
	 * interval.
	 * Halts when the _running member becomes false.
	 */
	void loop();

	/* Holds the average fps over the class' lifespan */
	float avg_fps;

private:
	std::vector<System *> _systems;
	bool *_running;

	/* the frame duration in milliseconds */
	float _frametime;
};
