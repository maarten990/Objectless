#include <typeinfo>
#include <typeindex>
#include <set>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include "Engine/Component.h"
#include "Engine/EntityManager.h"

using namespace std;

EntityManager::EntityManager(shared_ptr<ComponentManager> mgr)
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
            for (auto &system : system_pair.second) {
                system->notify_created(_next_id);
            }
        }
    }

    _next_id += 1;
    return _next_id - 1;
}

void EntityManager::remove(unsigned int id)
{
    /* notify each system that has a reference to this entity by
     * TODO: maybe store some extra data so that this can be done more
     * efficiently */
    for (auto &type_ptr_pair : _entities[id]) {
        remove_component(id, type_ptr_pair.first);
    }

    _entities.erase(id);
}

void EntityManager::add_component(unsigned int id, type_index type)
{
    _entities[id][type] = _component_mgr->construct(type);

    /* collect the entity's types */
    set<type_index> types;
    for (auto &pair : _entities[id]) {
        types.insert(pair.first);
    }

    /* check if this this entity should now be added to new systems */
    for (auto &system_pair : _systems) {
        set<type_index> required = system_pair.first;
        /* to prevent systems getting notified multiple times about the same
         * entry, make sure that this is a new notification by checking that the
         * newly added component is one of the required types */
        if (compare(required, types) && required.find(type) != required.end()) {
            for (auto &system : system_pair.second) {
                system->notify_created(id);
            }
        }
    }
}

void EntityManager::remove_component(unsigned int id, type_index type)
{
    /* notify each system that requests this type about this entity losing it */
    for (auto &system_pair : _systems) {
        if (system_pair.first.find(type) != system_pair.first.end()) {
            for (auto &system : system_pair.second) {
                system->notify_destroyed(id);
            }
        }
    }

    _entities[id].erase(type);
}

void EntityManager::register_system(shared_ptr<System> system,
        set<type_index> components)
{
	_systems[components].insert(system);
}


string EntityManager::pretty(unsigned int id)
{
    ostringstream str;
    str << "Entity " << id << ":\n";

    for (auto type_ptr_pair : _entities[id]) {
        str << "\t" << type_ptr_pair.first.name() << "\t" <<
            type_ptr_pair.second << endl;
    }

    return str.str();
}

shared_ptr<Component> EntityManager::find_component(unsigned int entity_id,
	type_index component_type)
{
	auto entity = _entities.find(entity_id);
	if (entity == std::end(_entities)) {
		return nullptr;
	}

	auto component = entity->second.find(component_type);
	if (component == std::end(entity->second)) {
		return nullptr;
	}

	return component->second;
}
