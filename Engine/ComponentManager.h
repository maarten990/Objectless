#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <functional>
#include <map>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <string>
#include "Component.h"
#include "System.h"

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
		std::type_index type(typeid(T));
		_constructors[type] = [] { return new T(); };
		_type_indices.insert(std::make_pair(type.name(), type));
	}

	/* Return a pointer to the type_index corresponding to its string
	 * representation. Returns NULL for an unknown/invalid name. */
	std::type_index *index_from_name(const std::string& name);

private:
	std::map<type_index, std::function<Component*()> > _constructors;

	/* a mapping of type_info strings to a corresponding type_index object */
	std::map<string, std::type_index> _type_indices;
};

#endif
