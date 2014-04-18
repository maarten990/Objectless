#ifndef KEYBOARDINPUTSYSTEM
#define KEYBOARDINPUTSYSTEM 
#include <SDL.h>
#include <vector>
#include "Component.h"

using namespace std;

class KeyBoardInputSystem
{
public:
	KeyBoardInputSystem();
	~KeyBoardInputSystem();
    void handleKeyEvent(SDL_Keycode k);

};

#endif
