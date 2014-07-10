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

type_index *ComponentManager::index_from_name(const std::string& name)
{
	auto it = _type_indices.find(name);

	if (it != std::end(_type_indices))
		return &it->second;
	else
		return nullptr;
}
