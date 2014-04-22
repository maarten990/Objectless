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
	void register_component(type_index type,
			function<Component*()> func);
	Component *construct(type_index type);

	void register_system(System *system, type_index type);
	
private:
	map<type_index, function<Component*()> > _constructors;
	map<type_index, vector<System*> > _systems;
};

#endif
