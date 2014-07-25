#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <algorithm>
#include "Component.h"
#include "Engine/EntityManager.h"

struct ComponentData;
class EntityManager;

/* Superclass for all systems */
class System {
public:
	virtual ~System() {};

	/* Performs one frame worth of calculations. Time since last frame in seconds
	 * given as the delta_time parameter. */
	virtual void step(float delta_time) = 0;

	/* Gets called when an entity is created which should be handled by the
	 * system.
	 * See also: ComponentManager::register */
	virtual void notify_created(unsigned int entity_id) {
		_entities.push_back(entity_id);
	}

	/* Gets called when an entity is about to be destroyed which should be
	 * handled by the system.
	 * See also: ComponentManager::register */
	virtual void notify_will_destroy(unsigned int entity_id) {
		auto iterator = std::find(_entities.begin(), _entities.end(), entity_id);

		/* make sure the entity is actually available */
		if (iterator != _entities.end())
			_entities.erase(iterator);
	}

	const std::vector<unsigned int> getEntities() const { return _entities; }

protected:
	/* List of entities that get acted on by the system. */
	std::vector<unsigned int> _entities;
};

#endif
