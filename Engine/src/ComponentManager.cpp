#include <string>
#include "ComponentManager.h"
#include "Component.h"
#include "System.h"

using namespace std;

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

type_index *ComponentManager::index_from_name(string name)
{
	map<string, type_index>::iterator it = _type_indices.find(name);

	if (it != _type_indices.end())
		return &it->second;
	else
		return nullptr;
}
