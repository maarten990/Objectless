#include <typeinfo>
#include <typeindex>
#include <set>
#include <algorithm>
#include <iterator>
#include "Component.h"
#include "EntityManager.h"

using namespace std;

EntityManager::EntityManager(ComponentManager *mgr)
{
    _next_id = 0;
    _component_mgr = mgr;
}

EntityManager::~EntityManager()
{
}

/* TODO: this can probably be more efficient with a better datastructure */
bool EntityManager::compare(set<type_index> required, set<type_index> available)
{
    set<type_index> substracted;

    /* the required components are available when (required - available = empty
     * set) */
    set_difference(required.begin(), required.end(),
            available.begin(), available.end(),
            inserter(substracted, substracted.end()));

    return substracted.size() == 0;
}

unsigned int EntityManager::add(set<type_index> types)
{
    /* create the entity */
    for (auto &type : types) {
        // Possible: Could add second argument _next_id
        _entities[_next_id][type] = _component_mgr->construct(type);
    }

    /* Notify the interested systems if the correct components are present. */
    for (auto &system_pair : _systems) {
        /* each component requested by the system should be present in types */
        if (compare(system_pair.first, types)) {
            for (System *system : system_pair.second) {
                system->notify_created(_next_id);
            }
        }
    }

    _next_id += 1;
    return _next_id - 1;
}

void EntityManager::remove(unsigned int id)
{
    map<type_index, Component*> entity = _entities[id];
    for (auto &component_pair : entity) {
        delete component_pair.second;
    }

    _entities.erase(id);
}

void EntityManager::register_system(System* system, set<type_index> components)
{
	_systems[components].insert(system);
}
