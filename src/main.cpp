#include <SDL.h>
#include <iostream>
#include <vector>
#include "GraphicsSystem.h"
#include "KeyBoardInputComponent.h"
#include "EventSystem.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include <map>
#include <functional>

const int FPS = 60;
const float FRAMEDURATION = (1.0 / 60) * 1000; // frame duration in ms


void test_function()
{
    std::cout<<"The right button was pressed"<<std::endl;;
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

    // TODO: this bool pointer that gets passed around to two classes sucks
    bool running = true;

    // Initialize entity manager and add first entity
    EntityManager e;
    std::vector<Component*> entity;
    std::map<SDL_Keycode, std::function<void()>> keys;
    std::function<void()> test = test_function;
    keys[SDLK_w] = test; 
    KeyboardInput k(keys);
    entity.push_back(&k);

    e.entityFactory(entity);

    SystemManager manager(60, &running);

    manager.add( new GraphicsSystem() );
    manager.add( new EventSystem(&running) );

    manager.loop();

    return 0;
}
