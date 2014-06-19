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

#include "Components.h"
#include "GraphicsSystem.h"
#include "KeyBoardInputComponent.h"
#include "EventSystem.h"

#ifdef WITH_LUA
#include "lua/REPLSystem.h"
#endif

#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 100

/* required to compile on Windows */
#undef main

const int FPS = 60;
const float FRAMEDURATION = (1.0 / 60) * 1000; // frame duration in ms


void test_function()
{
    std::cout<<"The right button was pressed"<<std::endl;;
}

void initialize_components(ComponentManager *mgr)
{
    mgr->register_component<KeyboardInput>();
    mgr->register_component<Graphics>();
    mgr->register_component<Position>();
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
    initialize_components(&component_mgr);
    EntityManager e(&component_mgr);

    // TODO: this bool pointer that gets passed around to two classes sucks
    bool running = true;

    GraphicsSystem *graphics = new GraphicsSystem();
    EventSystem *events = new EventSystem(&running);

    /* register systems */
    e.register_system(graphics, {Graphics::id(), Position::id()});
    e.register_system(events, {KeyboardInput::id()});

    std::set<type_index> player_components = {KeyboardInput::id(), Graphics::id(), Position::id()};
    unsigned int player = e.add(player_components);

    /* graphics stuff */
    SDL_Texture* t = graphics->loadTexture("../images/ball.bmp");
    e.get<Graphics>(player)->texture = t;
    /* TODO: extract this info during texture creation */
    e.get<Graphics>(player)->width = 20;
    e.get<Graphics>(player)->height = 20;

    e.get<Position>(player)->x = 50;
    e.get<Position>(player)->y = 75;
    e.get<Position>(player)->rotation = 0;

    /* keyboard stuff */
    std::map<SDL_Keycode, std::function<void()>> keys;
    keys[SDLK_w] = [&e, &player]() {e.get<Position>(player)->y -= 5; }; 
    keys[SDLK_a] = [&e, &player]() {e.get<Position>(player)->x -= 5; }; 
    keys[SDLK_s] = [&e, &player]() {e.get<Position>(player)->y += 5; }; 
    keys[SDLK_d] = [&e, &player]() {e.get<Position>(player)->x += 5; }; 
    keys[SDLK_h] = [&t, &e, &player]() {
        if (e.get<Graphics>(player))
            e.remove_component(player, Graphics::id());
        else {
            e.add_component(player, Graphics::id());
            e.get<Graphics>(player)->texture = t;
            e.get<Graphics>(player)->width = 20;
            e.get<Graphics>(player)->height = 20;
        }
    };

    e.get<KeyboardInput>(player)->keybinds = keys;

    SystemManager manager(60, &running, &e);

    manager.add(graphics);
    manager.add(events);

#ifdef WITH_LUA
    /* add a REPL if the input is redirected */
    REPLSystem *repl = new REPLSystem(&e, &component_mgr);
    if (!isatty(fileno(stdin))) {
        manager.add(repl);
    }
#endif

    manager.loop();

    SDL_Quit();

    return 0;
}
