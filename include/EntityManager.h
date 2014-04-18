#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <vector>
#include <iostream>
#include "Component.h"

using namespace std;

extern vector<vector<Component*>> entities;

class EntityManager
{
public:
    // TODO instead of specifying each entity by its component create an id for
    // each entity that specifies for each component if it is on or off

    // adds an entity, expressed by its components, to the list of entities
    void entityFactory(vector<Component*> entity);
};

#endif
