#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <vector>
#include <iostream>
#include "Component.h"

class EntityManager
{
public:
    // TODO instead of specifying each entity by its component create an id for
    // each entity that specifies for each component if it is on or off
    vector<vector<Component>> entities;

    // adds an entity, expressed by its components, to the list of entities
    void  EntityFactory(vector<Component> entity);
};

#endif
