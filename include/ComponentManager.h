#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <functional>
#include <map>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include "Component.h"
#include "System.h"

using namespace std;

/* Provides an interface for the creation and destruction of components. */
class ComponentManager {
public:
	ComponentManager();
	~ComponentManager();

	/* Creates a factory function for the supplied component id. Has to be
	 * called once for every type of component. */
	template <typename T>
	void register_component() {
		_constructors[T::id()] = []() { return new T(); };
	}

	/* Create a component of the given type, belonging to a given entity. */
	Component *construct(type_index type, unsigned int id);

	/* Register a system as being interested in a certain type of component. The
	 * system will be notified when such a component is added to or removed from
	 * a component. */
	void register_system(System* system, type_index type);
	
private:
	map<type_index, function<Component*()> > _constructors;
	map<type_index, vector<System*> > _systems;
};

#endif
