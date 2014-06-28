#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <map>
#include <set>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <string>
#include "Component.h"
#include "ComponentManager.h"

using namespace std;

/* Provides an interface to create, destroy or modify an entity and its
 * components. */
class EntityManager
{
public:
    EntityManager(shared_ptr<ComponentManager> mgr);
    ~EntityManager();

    /* Adds an entity and returns a unique integer by which to identify it.
     * The argument consists of a set of ids representing the components
     * to be added to the entity.
     */
    unsigned int add(std::set<type_index> types);

    /* Remove the specified entity and deallocate its memory. */
    void remove(unsigned int id);

    /* Add or remove a component from an entity. Every entity is assumed to have
     * a maximum of instance of each type of component. */
    void add_component(unsigned int id, type_index type);
    void remove_component(unsigned int id, type_index type);

    /* returns a pointer to a component belonging to a given entity */
    template <typename T>
    shared_ptr<T> get(unsigned int entity_id) {
	return static_pointer_cast<T>(find_component(entity_id, T::id()));
    }

    /* Register a system as being interested in a certain combination of
     * component. The system will be notified when an entity containing these
     * components is created, destroyed, or has changed its component layout
     * such that it no longer contains the required ones. */
	void register_system(shared_ptr<System> system, set<type_index> components);

    /* Return a pretty string containing info about an entity. */
    string pretty(unsigned int id);

private:
    /* compare two sets; return true if each element of 'available' occurs in
     * 'required' */
    bool compare(set<type_index> required, set<type_index> available);

		/* Find component belonging to the specified entity. Returns null if the entity
		 * does not have a component of that type. */
		shared_ptr<Component> find_component(unsigned int entity_id, type_index component_type);

    /* entities are stored with their components in a database-like table, where
     * rows are entities and columns are components.
     * e.g. _entities[entity_id][component_id] will return a pointer to a
     * component of type component_id possessed by a certain entity; if the
     * entity does not contain such a component, NULL is returned.
     *
     * The table is implemented as a nested dictionary.
     */
      map<unsigned int, map<type_index, shared_ptr<Component> > > _entities;
      unsigned int _next_id;
      shared_ptr<ComponentManager> _component_mgr;
      map<set<type_index>, set<shared_ptr<System> > > _systems;
};

#endif
