#include <typeinfo>
#include <typeindex>
#include <vector>
#include "Component.h"
#include "EntityManager.h"

EntityManager::EntityManager(ComponentManager *mgr)
{
    _next_id = 0;
    _component_mgr = mgr;
}

EntityManager::~EntityManager()
{
}

unsigned int EntityManager::add(std::vector<type_index> types)
{
    for (auto &type : types) {
        // Possible: Could add second argument _next_id
        _entities[_next_id][type] = _component_mgr->construct(type, _next_id);
    }

    _next_id += 1;
    return _next_id - 1;
}

void EntityManager::remove(unsigned int id)
{
    _entities.erase(id);
}
