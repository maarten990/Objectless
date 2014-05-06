#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <map>
#include <typeinfo>
#include <typeindex>
#include "Component.h"
#include "ComponentManager.h"

using namespace std;

/* Provies an interface to create, destroy or modify an entity and its
 * components. */
class EntityManager
{
public:
    EntityManager(ComponentManager *mgr);
    ~EntityManager();

    /* Adds an entity and returns a unique integer by which to identify it.
     * The argument consists of a vector of ids representing the components
     * to be added to the entity.
     */
    unsigned int add(std::vector<type_index> types);

    /* Remove the specified entity and deallocate its memory. */
    void remove(unsigned int id);

    /* returns a pointer to a component belonging to a given entity */
    template <typename T>
    T *get(unsigned int entity_id) {
        return static_cast<T*>(_entities[entity_id][T::id()]);
    }

private:
    /* entities are stored with their components in a database-like table, where
     * rows are entities and columns are components.
     * e.g. _entities[entity_id][component_id] will return a pointer to a
     * component of type component_id possessed by a certain entity; if the
     * entity does not contain such a component, NULL is returned.
     *
     * The table is implemented as a nested dictionary.
     */
      map<unsigned int, map<type_index, Component*> > _entities;
      unsigned int _next_id;
      ComponentManager *_component_mgr;
};

#endif
