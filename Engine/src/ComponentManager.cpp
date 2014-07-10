#include <string>
#include "Engine/ComponentManager.h"
#include "Engine/Component.h"
#include "Engine/System.h"

using namespace std;

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

{

type_index *ComponentManager::index_from_name(string name)
{
	map<string, type_index>::iterator it = _type_indices.find(name);

	if (it != _type_indices.end())
		return &it->second;
	else
		return nullptr;
}
