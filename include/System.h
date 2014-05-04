#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include "Component.h"

class EntityManager;


class System {
public:
	virtual ~System() {};
	virtual void step(unsigned int dt, EntityManager* em) = 0;

	void notify_created(unsigned int entity) {
		_entities.push_back(entity);
	}

	void notify_destroyed(unsigned int removed_entity) {
		for (auto entity = _entities.begin();
				entity != _entities.end(); ++entity) {
			if (*entity == removed_entity) {
				_entities.erase(entity);
			}
		}
	}

protected:
	std::vector<unsigned int> _entities;
};

#endif
