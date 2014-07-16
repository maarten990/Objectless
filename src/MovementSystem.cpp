#include "MovementSystem.h"
#include "Components.h"
#include "Engine/EntityManager.h"
#include <stdio.h>

MovementSystem::MovementSystem(EntityManager *em)
	: _em(em)
{
}

void MovementSystem::step(float dt)
{
	for (unsigned int id: _entities) {
		Movement *mv = _em->get_component<Movement>(id);
		Transform *t = _em->get_component<Transform>(id);

		t->x += mv->x_speed * mv->x_multiplier;
		t->y += mv->y_speed * mv->y_multiplier;
	}
}
