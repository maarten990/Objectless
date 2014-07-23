#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include "SDL.h"
#include "Engine/Component.h"
#include <memory>

#include <Box2D/Common/b2Math.h>

class Texture;

struct Graphics : Component {
	std::shared_ptr<Texture> texture;
	//Should this component be visible?
	bool is_visible = true;
};

struct Transform : Component {
	b2Vec2 position = b2Vec2(0.0f, 0.0f);
	b2Rot rotation = b2Rot(0.0f);
	//Note: Does not affect physics, only visuals.
	b2Vec2 scale = b2Vec2(1.0f, 1.0f);
};

#endif
