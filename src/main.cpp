#include <SDL.h>
#include <iostream>
#include <set>
#include <map>
#include <functional>
#include <Box2D/Box2D.h>

#ifdef __linux
#include <unistd.h>
#endif

#include "Engine/EntityManager.h"
#include "Engine/SystemManager.h"
#include "Engine/ComponentManager.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Physics/Box2DDebugDraw.h"
#include "GeometryDrawer.h"

#include "Components.h"
#include "GraphicsSystem.h"
#include "KeyBoardInputComponent.h"
#include "EventSystem.h"
#include "Engine/Physics/PhysicsBodyComponent.h"
#include "Engine/Graphics/Texture.h"

#ifdef WITH_LUA
#include "lua/REPLSystem.h"
#endif

/* required to compile on Windows */
#undef main


const unsigned int window_width = 800;
const unsigned int window_height = 640;

void initialize_components(ComponentManager& mgr)
{
	mgr.register_component<KeyboardInput>();
	mgr.register_component<Graphics>();
	mgr.register_component<Transform>();
}

void movePlayer(EntityManager& em, unsigned int player_id, b2Vec2 translation, GeometryDrawer& drawer)
{
	b2Body& body = *em.get_component<PhysicsBodyComponent>(player_id)->body;
	translation = b2Mul(body.GetTransform().q, translation);
	translation *= 10000.0f;
	//body.ApplyForceToCenter(translation, true);
	body.ApplyLinearImpulse(translation, body.GetWorldCenter(), true);
	translation.Normalize();
	translation *= 25.0f;
	drawer.draw_line(body.GetPosition().x, body.GetPosition().y, body.GetPosition().x + translation.x, body.GetPosition().y + translation.y);
}

void rotatePlayer(EntityManager& em, unsigned int player_id, float degrees)
{
	b2Body& body = *em.get_component<PhysicsBodyComponent>(player_id)->body;
	b2Transform transform = body.GetTransform();
	//body.SetTransform(transform.p, transform.q.GetAngle() + degrees * 0.1f);
	//body.ApplyTorque(degrees * 100.0f, true);
	body.ApplyAngularImpulse(degrees * 3000.0f, true);
}

void createPlayer(EntityManager& em, GraphicsSystem& graphics, GeometryDrawer& drawer)
{
	unsigned int player = em.create_entity<KeyboardInput, Graphics, Transform, PhysicsBodyComponent>();
	b2Body& body = *em.get_component<PhysicsBodyComponent>(player)->body;
	b2PolygonShape* shape = new b2PolygonShape();
	shape->SetAsBox(10.0f, 10.0f, b2Vec2(10.0f, 10.0f), 0.0f);
	body.CreateFixture(shape, 1.0f);
	body.SetType(b2_dynamicBody);
	body.SetGravityScale(0.0f);
	body.SetLinearDamping(1.0f);
	body.SetAngularDamping(1.1f);
	body.SetTransform(b2Vec2(250.0f, 250.0f), 0.0f);

	/* graphics stuff */
	const char* path = "C:/Users/batman/Pictures/Puke.bmp";
	auto texture = std::make_shared<Texture>(*graphics.getRenderer(), path);
	em.get_component<Graphics>(player)->texture = texture;

	/* keyboard stuff */
	std::map<SDL_Keycode, std::function<void()>> keys;
	keys[SDLK_w] = [&em, &drawer, player]() { movePlayer(em, player, b2Vec2(0.0f, -5.0f), drawer); };
	keys[SDLK_s] = [&em, &drawer, player]() { movePlayer(em, player, b2Vec2(0.0f, 5.0f), drawer); };

	keys[SDLK_a] = [&em, player]() { rotatePlayer(em, player, -1.0f); };
	keys[SDLK_d] = [&em, player]() { rotatePlayer(em, player, 1.0f); };

	keys[SDLK_h] = [texture, &em, player]() {
		Graphics& graphics = *em.get_component<Graphics>(player);
		graphics.is_visible = !graphics.is_visible;
	};

	em.get_component<KeyboardInput>(player)->keybinds = keys;

	em.get_component<Transform>(player)->scale *= 0.5f;
}

unsigned int createRB(EntityManager& em, const std::shared_ptr<Texture>& texture,
	b2Vec2 position = b2Vec2(200.0f, 200.0f), b2Vec2 scale = b2Vec2(1.0f, 1.0f))
{
	unsigned int id = em.create_entity<Transform, Graphics, PhysicsBodyComponent>();

	b2Vec2 half_size(texture->getWidth() * 0.5f * scale.x,
		texture->getHeight() * 0.5f * scale.y);

	b2PolygonShape* shape = new b2PolygonShape();
	shape->SetAsBox(half_size.x, half_size.y, half_size, 0.0f);

	b2Body& body = *em.get_component<PhysicsBodyComponent>(id)->body;
	body.SetType(b2_dynamicBody);
	body.SetLinearDamping(2.0f);
	body.SetAngularDamping(0.5f);
	body.CreateFixture(shape, 0.25f);
	body.SetTransform(position, 0.0f);

	em.get_component<Transform>(id)->scale = scale;

	em.get_component<Graphics>(id)->texture = texture;

	return id;
}

void createWorldEdges(EntityManager& em)
{
	unsigned int id = em.create_entity<Transform, PhysicsBodyComponent>();

	b2Body& body = *em.get_component<PhysicsBodyComponent>(id)->body;
	b2ChainShape* shape = new b2ChainShape();
	//These vertices are positioned at the edges of the window.
	b2Vec2 vertices[4] = 
	{
		b2Vec2(0.0f, 0.0f),
		b2Vec2((float)window_width, 0.0f),
		b2Vec2((float)window_width, (float)window_height),
		b2Vec2(0.0f, (float)window_height),
	};
	shape->CreateLoop(vertices, 4);
	body.CreateFixture(shape, 1.0f);
	body.SetType(b2_staticBody);
}

int main()
{
	// initialize sdl
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER |
		SDL_INIT_AUDIO) < 0) {
		std::cout << "Could not initialize SDL. SDL error: " <<
			SDL_GetError() << std::endl;
		return -1;
	}

	ComponentManager component_mgr;
	initialize_components(component_mgr);
	EntityManager em;

	// TODO: this bool pointer that gets passed around to two classes sucks
	bool running = true;

	GraphicsSystem *graphics = new GraphicsSystem(&em, window_width, window_height);
	EventSystem *events = new EventSystem(&running, &em);
	GeometryDrawer* geometryDrawer = new GeometryDrawer(graphics->getRenderer(), graphics);
	PhysicsWorld* physics_world = new PhysicsWorld(em, b2Vec2(0.0f, 981.0f)); // todo use less insane gravity
	Box2DDebugDraw* box2d_debug_draw = new Box2DDebugDraw(graphics->getRenderer(), *graphics, physics_world->getWorld());
	box2d_debug_draw->SetFlags(
		b2Draw::e_shapeBit
		| b2Draw::e_jointBit
		//| b2Draw::e_aabbBit
		| b2Draw::e_pairBit
		| b2Draw::e_centerOfMassBit
		);
	box2d_debug_draw->SetFlags(0);
	physics_world->getWorld().SetDebugDraw(box2d_debug_draw);

	/* register systems */
	em.register_system<Graphics>(graphics);
	em.register_system<KeyboardInput>(events);
	em.register_system<Transform>(geometryDrawer);
	em.register_system<PhysicsBodyComponent>(physics_world);

	createPlayer(em, *graphics, *geometryDrawer);

	createWorldEdges(em);

	auto texture = std::make_shared<Texture>(*graphics->getRenderer(), "../images/crate64.jpg");
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			const b2Vec2 scale(1.0f, 1.0f);
			const b2Vec2 position(350.0f + texture->getWidth() * scale.x * i,
				200.0f + texture->getHeight() * scale.y * j);
			createRB(em, texture, position, scale);
		}
	}

	SystemManager manager(120, &running);

	manager.add(graphics);
	manager.add(events);
	manager.add(geometryDrawer);
	manager.add(physics_world);

#ifdef WITH_LUA
	/* add a REPL if the input is redirected */
	REPLSystem *repl = new REPLSystem(&em, &component_mgr);
	if (!isatty(fileno(stdin))) {
		manager.add(repl);
	}
#endif

	manager.loop();

	SDL_Quit();

	return 0;
}