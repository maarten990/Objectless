#include "Component.h"
#include "EntityManager.h"
#include <iostream>
#include <vector>
void EntityManager::EntityFactory(vector<Component> entity)
{
    // Add entity to the vector of entities
    entities.push_back(entity);
}
