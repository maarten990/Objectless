#include <SDL.h>
#include <iostream>
#include <vector>
#include "GraphicsSystem.h"
#include "KeyBoardInputComponent.h"
#include "EventSystem.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"
#include <map>
#include <functional>
#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 100


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

    component_mgr.register_system(graphics, Graphics::id());
    component_mgr.register_system(graphics, Position::id());
    component_mgr.register_system(events, KeyboardInput::id());

    std::vector<type_index> player_components = {KeyboardInput::id(), Graphics::id(), Position::id()};
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
    e.get<KeyboardInput>(player)->keybinds = keys;

    SystemManager manager(60, &running, &e);

    manager.add(graphics);
    manager.add(events);

    manager.loop();

    SDL_Quit();

    return 0;
}
