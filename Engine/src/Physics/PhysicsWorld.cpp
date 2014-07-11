#include "Engine/Physics/PhysicsWorld.h"
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include "Timer.h"
#include <windows.h>


PhysicsWorld::PhysicsWorld(const b2Vec2& gravity /*= b2Vec2(0.0f, -9.81f)*/)
	: world(gravity)
{
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.position.Set(250.0f, 250.0f);
	def.gravityScale = 0.0f;
	b2Body& body = *world.CreateBody(&def);
	b2Shape* shape = new b2CircleShape();
	shape->m_radius = 20.0f;
	body.CreateFixture(shape, 1.0f);
}

void PhysicsWorld::step(float delta_time)
{
	accumulated_delta_time += delta_time;

	unsigned int num_steps = 0;
	Timer timer;

	while (accumulated_delta_time > fixed_delta_time)
	{
		accumulated_delta_time -= fixed_delta_time;
		stepWorld();
		++num_steps;
	}

	if (num_steps != 0)
	{
		//update visual positions???????
		double duration_ms = timer.seconds_since_start() * 1000.0;
		printf("Physics stepped %u times in %.2f ms (avg %.2f ms per step)\n",
			num_steps, duration_ms, duration_ms / num_steps);
	}
}

void PhysicsWorld::stepWorld()
{
	world.Step(fixed_delta_time, num_velocity_iterations, num_position_iterations);
}
