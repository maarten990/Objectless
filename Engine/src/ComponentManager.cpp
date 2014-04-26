#include "ComponentManager.h"
#include "Component.h"
#include "System.h"

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

Component *ComponentManager::construct(type_index type, unsigned int entity_id)
{
	Component *component = _constructors[type]();

	for (auto &system : _systems[type]) {
		system->notify_created(entity_id);
	}

	return component;
}

void ComponentManager::register_system(System *system, type_index type)
{
	_systems[type].push_back(system);
}
