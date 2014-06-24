#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <algorithm>
#include "Component.h"

class EntityManager;

/* Superclass for all systems */
class System {
public:
	virtual ~System() {};

	/* Performs one frame worth of calculations. Time since last frame in seconds
	 * given as the delta_time parameter. */
	virtual void step(float delta_time, EntityManager* em) = 0;

	/* Gets called when an entity is created which should be handled by the
	 * system.
	 * See also: ComponentManager::register */
	void notify_created(unsigned int entity) {
		_entities.push_back(entity);
	}

	/* Gets called when an entity is destroyed which should be handled by the
	 * system.
	 * See also: ComponentManager::register */
	void notify_destroyed(unsigned int removed_entity) {
		auto iterator = std::find(_entities.begin(), _entities.end(),
				removed_entity);

		/* make sure the entity is actually available */
		if (iterator != _entities.end())
			_entities.erase(iterator);
	}

protected:
	/* List of entities that get acted on by the system. */
	std::vector<unsigned int> _entities;
};

#endif
