#include <typeinfo>
#include <typeindex>
#include <vector>
#include <algorithm>
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
        _entities[_next_id][type] = _component_mgr->construct(type);
    }

    /* Notify the interested systems if the correct components are present.
     * TODO: this can be WAY more efficient with a better datatype, this is
     * slow-ass placeholder code */
    for (auto &system_pair : _systems) {
        /* each component requested by the system should be present in types */
        vector<type_index> required_components = system_pair.first;
        bool all_present;

        all_present = all_of(required_components.begin(),
                required_components.end(),
                [&types](type_index req_type) {
                    return std::find(types.begin(), types.end(),
                        req_type) != types.end();
                });

        if (all_present) {
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

void EntityManager::register_system(System* system, vector<type_index> components)
{
	_systems[components].push_back(system);
}
