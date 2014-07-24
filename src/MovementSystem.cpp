#include "MovementSystem.h"
#include "Components.h"
#include "Engine/EntityManager.h"
#include "Engine/Physics/PhysicsBodyComponent.h"
#include <Box2D/Dynamics/b2Body.h>

MovementSystem::MovementSystem(EntityManager *em)
	: _em(em)
{
}

void MovementSystem::step(float /*dt*/)
{
	for (unsigned int id: _entities) {
		Movement *mv = _em->get_component<Movement>(id);
		PhysicsBodyComponent* rb = _em->get_component<PhysicsBodyComponent>(id);

		//todo remove this check, will be redundant when System::notify_created
		//works correctly.
		if (mv == nullptr || rb == nullptr)
		{
			return;
		}
		b2Vec2 linear_velocity(mv->speed);
		linear_velocity.x *= mv->speed_multiplier.x;
		linear_velocity.y *= mv->speed_multiplier.y;

		//todo don't do this, using a more sensible world scale should remove the
		//need for this.
		linear_velocity *= 10000.0f;

		rb->body->SetLinearVelocity(linear_velocity);
	}
}
