#include "Component.h"
#include "EntityManager.h"
#include <iostream>
#include "KeyBoardInputComponent.h"
#include <vector>

vector<vector<Component*>> entities;
void EntityManager::entityFactory(vector<Component*> entity)
{
    // Add entity to the vector of entities
    entities.push_back(entity);
}
