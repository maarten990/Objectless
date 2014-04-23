#include "ComponentManager.h"
#include "Component.h"

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
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
