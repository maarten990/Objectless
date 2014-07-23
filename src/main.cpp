#include <SDL.h>
#include <iostream>
#include <set>
#include <map>
#include <functional>

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
	PhysicsWorld* physics_world = new PhysicsWorld();
	Box2DDebugDraw* box2d_debug_draw = new Box2DDebugDraw(graphics->getRenderer(), *graphics, physics_world->getWorld());
	box2d_debug_draw->SetFlags(b2Draw::e_aabbBit | b2Draw::e_jointBit | b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
	physics_world->getWorld().SetDebugDraw(box2d_debug_draw);

	/* register systems */
	em.register_system<Graphics, Transform>(graphics);
	em.register_system<KeyboardInput>(events);
	em.register_system<Transform>(geometryDrawer);

	unsigned int player = em.create_entity<KeyboardInput, Graphics, Transform>();

	/* graphics stuff */
	SDL_Texture* t = graphics->loadTexture("../images/ball.bmp");
	em.get_component<Graphics>(player)->texture = t;
	/* TODO: extract this info during texture creation */
	em.get_component<Graphics>(player)->width = 20;
	em.get_component<Graphics>(player)->height = 20;

	em.get_component<Transform>(player)->x = 50;
	em.get_component<Transform>(player)->y = 75;

	/* keyboard stuff */
	std::map<SDL_Keycode, std::function<void()>> keys;
	keys[SDLK_w] = [&em, &player]() {em.get_component<Transform>(player)->y -= 5; };
	keys[SDLK_a] = [&em, &player]() {em.get_component<Transform>(player)->x -= 5; };
	keys[SDLK_s] = [&em, &player]() {em.get_component<Transform>(player)->y += 5; };
	keys[SDLK_d] = [&em, &player]() {em.get_component<Transform>(player)->x += 5; };
	keys[SDLK_h] = [&t, &em, &player]() {
		if (em.get_component<Graphics>(player))
			em.remove_component<Graphics>(player);
		else {
			em.add_component<Graphics>(player);
			em.get_component<Graphics>(player)->texture = t;
			em.get_component<Graphics>(player)->width = 20;
			em.get_component<Graphics>(player)->height = 20;
		}
	};

	em.get_component<KeyboardInput>(player)->keybinds = keys;

	SystemManager manager(60, &running);

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