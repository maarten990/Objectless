#ifndef KEYBOARDINPUTCOMPONENT_H
#define KEYBOARDINPUTCOMPONENT_H

#include <SDL.h>
#include <typeindex>
#include <typeinfo>
#include <map>

#include "Engine/Component.h"

using namespace std;

// Entities with a keypress component specify the keys that it responds
// to and a pointer to the function it might react to

struct KeyboardInput : public Component {

    KeyboardInput() {}
    KeyboardInput(map<SDL_Keycode, std::function<void()>> keys) {
        keybinds = keys;
    }

    static type_index id() {
        return typeid(KeyboardInput);
    }

    // Map contains possible key presses and their consequence (function
    // pointer)
    map<SDL_Keycode, std::function<void()>> keybinds;

};
#endif
