#include "System.h"
#include <vector>
#include "EntityManager.h"

class SystemManager {
public:
	SystemManager(int fps, bool *running, EntityManager *entity_manager);
	~SystemManager();

	/* Add a system.
	 * The argument should be a pointer to a heap-allocated system.
	 * The pointer will be freed in SystemManager's destructor. */
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

    EntityManager *_entity_manager;


};
