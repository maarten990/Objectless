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

class ComponentManager {
public:
	ComponentManager();
	~ComponentManager();

	template <typename T>
	void register_component() {
		_constructors[T::id()] = []() { return new T(); };
	}

	Component *construct(type_index type);

	void register_system(System *system, type_index type);
	
private:
	map<type_index, function<Component*()> > _constructors;
	map<type_index, vector<System*> > _systems;
};

#endif
