#pragma once

#include "Engine/Component.h"

class b2Body;


class PhysicsBodyComponent : public Component
{
public:
	b2Body* body = nullptr;
};