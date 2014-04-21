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
#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 100


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

    GraphicsSystem* graphics = new GraphicsSystem();

    // Initialize entity manager and add first entity
    EntityManager e;
    std::vector<Component*> entity;
    std::map<SDL_Keycode, std::function<void()>> keys;
    std::function<void()> test = test_function;
    keys[SDLK_w] = test; 
    KeyboardInput k(keys);
    entity.push_back(&k);

    SDL_Texture* t = graphics->loadTexture("../images/ball.bmp");
    // Add graphics component as well to our entity
    Graphics g(t);
    Position p;
    p.x = 5;
    p.y = 5;
    p.rotation = 0;
    entity.push_back(&g);
    entity.push_back(&p);


    e.entityFactory(entity);

    SystemManager manager(60, &running);

    manager.add( graphics);
    manager.add( new EventSystem(&running) );

    manager.loop();

	SDL_Quit();

    return 0;
}
