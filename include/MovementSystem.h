#pragma once

#include "Engine/System.h"
#include "Engine/EntityManager.h"

class MovementSystem : public System
{
public:
	MovementSystem(EntityManager *em);
	void step(float dt);

private:
	EntityManager *_em;
};
