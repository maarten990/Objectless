#include "Component.h"
#include "EntityManager.h"

EntityManager::EntityManager()
{
    num_components = 2;
}

EntityManager::~EntityManager()
{
    /* deallocate each entity and its components */
    for (auto keyval : _entities) {
        this->remove(keyval.first);
    }
}

int EntityManager::add(const char *entity)
{
    _entities[_next_index] = new Component*[num_components];
    
    for (int i = 0; i < num_components; ++i) {
        if (entity[i] != '0') {
            // TODO: construct component
        } else {
            _entities[_next_index][i] = NULL;
        }
    }

    _next_index += 1;
    return _next_index - 1;
}

void EntityManager::remove(unsigned int id)
{
    for (int i = 0; i < num_components; ++i) {
        if (_entities[id][i] != NULL) {
            delete _entities[id][i];
        }
    }

    _entities.erase(id);
}
