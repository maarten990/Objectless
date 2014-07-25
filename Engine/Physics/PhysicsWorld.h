#pragma once

#include <stdint.h>
#include <Box2D/Dynamics/b2World.h>
#include "Engine/System.h"
#include "Engine/Physics/Box2DDebugDraw.h"


class PhysicsWorld : public System
{
public:
	PhysicsWorld(EntityManager& entity_manager, const b2Vec2& gravity = b2Vec2(0.0f, -9.81f));

	PhysicsWorld& operator=(const PhysicsWorld& other) = delete;

	void step(float delta_time);

	b2World& getWorld() { return world; }

	void notify_created(unsigned int entity_id) override;

private:
	void stepWorld();

	/* Synchronize graphical transformation with physical transformation.
	 * This keeps graphics and physics synchronized.
	*/
	void syncTransforms();

	b2World world;
	//Box2DDebugDraw debug_drawer;

	//Controls the accuracy of the simulation.
	uint32_t num_velocity_iterations = 10;
	uint32_t num_position_iterations = 10;

	/* This is the fixed delta time that will always be used to step the physics world.
	 * Using a fixed delta time instead of a dynamic delta time causes the physics to behave
	 * in a more predictable manner.
	 */
	float fixed_delta_time = 1.0f / 100.0f;
	/* This variable stores how much real time has passed, and is used to update
	 * the physics world with a fixed time step.
	 * When this is greater than fixed_delta_time, the world is stepped.
	 */
	float accumulated_delta_time = 0.0f;

	EntityManager& entity_manager;
};