#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include "Component.h"

class System {
public:
	virtual ~System() {};
	virtual void step(unsigned int dt) = 0;

	void notify_created(Component *component) {
		_components.push_back(component);
	}

	void notify_destroyed(Component *removed) {
		for (auto component = _components.begin();
				component != _components.end(); ++component) {
			if (*component == removed) {
				_components.erase(component);
			}
		}
	}

protected:
	std::vector<Component*> _components;
};

#endif
