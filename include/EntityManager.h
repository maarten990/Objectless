#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <map>
#include <typeinfo>
#include <typeindex>
#include "Component.h"
#include "ComponentManager.h"

using namespace std;

class EntityManager
{
public:
    EntityManager(ComponentManager *mgr);
    ~EntityManager();

    /* Adds an entity and returns a unique integer by which to identify it.
     * The id is a combination of component ids which can be OR'd together.
     */
    unsigned int add(std::vector<type_index> types);

    /* Remove the specified entity and deallocate its memory. */
    void remove(unsigned int id);

    /* returns a pointer to a given entity */
    Component *get(unsigned int id, type_index component);

private:
    /* entities are stored with their components in a database-like table, where
     * rows are entities and columns are components.
     * e.g. _entities[entity][component_id] will return a pointer to a
     * component of type component_id possessed by a certain entity; if the
     * entity does not contain such a component, NULL is returned.
     */
      map<unsigned int, map<type_index, Component*> > _entities;
      unsigned int _next_id;
      ComponentManager *_component_mgr;
};

#endif
