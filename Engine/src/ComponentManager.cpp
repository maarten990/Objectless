#include "ComponentManager.h"
#include "Component.h"
#include "System.h"

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

Component *ComponentManager::construct(type_index type)
{
	Component *component = _constructors[type]();

	return component;
}
