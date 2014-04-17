#ifndef ENTITY_H
#define ENTITY_H value

#include <vector>
#include "Component.h"

class Entity
{
public:
	std::vector<Component*> components;
};

#endif
