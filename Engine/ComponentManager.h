#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <functional>
#include <map>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <memory>
#include "Component.h"
#include "System.h"

using namespace std;

/* Provides an interface for the creation and destruction of components. */
class ComponentManager {
public:
	ComponentManager();
	~ComponentManager();

	/* Creates a factory function for the supplied component id, and enters the
	 * component in the map of type_info string. Has to be called once for every
	 * type of component. */
	template <typename T>
	void register_component() {
		_constructors[T::id()] = []() { return new T(); };
		_type_indices.insert(make_pair(typeid(T).name(), T::id()));
	}

	/* Create a component of the given type, belonging to a given entity. */
	shared_ptr<Component> construct(type_index type);

	/* Return a pointer to the type_index corresponding to its string
	 * representation. Returns NULL for an unknown/invalid name. */
	type_index *index_from_name(string name);

private:
	map<type_index, function<Component*()> > _constructors;

	/* a mapping of type_info strings to a corresponding type_index object */
	map<string, type_index> _type_indices;
	
};

#endif
