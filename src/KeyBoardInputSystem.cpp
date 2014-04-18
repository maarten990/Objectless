#include "KeyBoardInputSystem.h"
#include "KeyBoardInputComponent.h"
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
void KeyBoardInputSystem::handleKeyEvent(SDL_KeyboardEvent keyEvent, vector<vector<Component>> entities) 
{
    // Check for each of the entities if the component can be downcast to
    // KeyBoardInputComponent. If so, try apply current keyboard event
    for (int i = 0; i < entities.size(); i++) {
        for (int j = 0; i < entities[i].size(); j++) {
            Component* c = &entities[i][j];
            if (KeyboardInput* kb = dynamic_cast<KeyboardInput*>(c)) {
                SDL_Keycode k = keyEvent.keysym.sym;
                std::function<void()> f = kb->keybinds[k];
                f();
            }
        
        }
    }
}

