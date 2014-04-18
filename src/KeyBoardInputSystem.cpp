#include "KeyBoardInputSystem.h"
#include "KeyBoardInputComponent.h"
#include "EntityManager.h"
#include "Component.h"
#include <SDL.h>
#include <vector>
#include <functional>

// TODO:
// Every system should have a reference to 
// all entities it acts on (no unnecessary looping) :D
KeyBoardInputSystem::KeyBoardInputSystem()
{
}

KeyBoardInputSystem::~KeyBoardInputSystem ()
{
}

// Handles all key presses of each of the entities
void KeyBoardInputSystem::handleKeyEvent(SDL_Keycode k) 
{
    // Check for each of the entities if the component can be downcast to
    // KeyBoardInputComponent. If so, try apply current keyboard event
    for (int i = 0; i < entities.size(); i++) {
        for (int j = 0; j < entities[i].size(); j++) {
            KeyboardInput* kb = dynamic_cast<KeyboardInput*>(entities[i][j]);
            if (kb) {
                // Check if key is in the dict. If so, apply function
                if(kb->keybinds.find(k) == kb->keybinds.end()) {
                }
                else {
                    std::function<void()> f = kb->keybinds[k];
                    f();
                }
            }
        }
    }
}

