#include "ComponentManager.h"
#include "Component.h"

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::register_component(type_index type,
		function<Component*(void*)> func)
{
	_constructors[type] = func;
}

Component *ComponentManager::construct(type_index type, void *data)
{
	return _constructors[type](data);
}
