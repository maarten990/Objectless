#include "ComponentManager.h"
#include "Component.h"

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::register_component(type_index type,
		function<Component*()> func)
{
	_constructors[type] = func;
}

Component *ComponentManager::construct(type_index type)
{
	Component *component = _constructors[type]();

	for (auto &system : _systems[type]) {
		system->notify_created(component);
	}

	return component;
}

void ComponentManager::register_system(System *system, type_index type)
{
	_systems[type].push_back(system);
}
