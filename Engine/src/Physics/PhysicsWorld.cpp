#include "Engine/Physics/PhysicsWorld.h"
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include "Engine/Physics/PhysicsBodyComponent.h"
#include "Engine/EntityManager.h"
#include "Components.h"


PhysicsWorld::PhysicsWorld(EntityManager& entity_manager, const b2Vec2& gravity /*= b2Vec2(0.0f, -9.81f)*/)
	: world(gravity)
	, entity_manager(entity_manager)
{
}

void PhysicsWorld::step(float delta_time)
{
	accumulated_delta_time += delta_time;

	unsigned int num_steps = 0;

	while (accumulated_delta_time > fixed_delta_time)
	{
		accumulated_delta_time -= fixed_delta_time;
		stepWorld();
		++num_steps;
	}

	if (num_steps != 0)
	{
		syncTransforms();
	}
}

void PhysicsWorld::stepWorld()
{
	world.Step(fixed_delta_time, num_velocity_iterations, num_position_iterations);
}

void PhysicsWorld::notify_created(unsigned int entity_id)
{
	System::notify_created(entity_id);

	//Create and link Box2D body to the component.
	PhysicsBodyComponent& component = *entity_manager.get_component<PhysicsBodyComponent>(entity_id);
	b2BodyDef def;
	//This makes it possible to get a pointer to a component directly from a b2Body.
	def.userData = &component;
	component.body = world.CreateBody(&def);
}

void PhysicsWorld::syncTransforms()
{
	b2Body* body = world.GetBodyList();
	do
	{
		const b2Transform& transform = body->GetTransform();
		PhysicsBodyComponent& component = *(PhysicsBodyComponent*)body->GetUserData();
		Transform& entity_transform = *entity_manager.get_component<Transform>(component.entity_id);
		entity_transform.position = transform.p;
		entity_transform.rotation = transform.q;

	} while ((body = body->GetNext()) != nullptr);
}
