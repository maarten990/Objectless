#include <string>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <lua.hpp>
#include "lua/REPLSystem.h"
#include "lua/lua_interface.h"

using namespace std;

#define LUA_GLOBAL_FUNCTION(state, func) \
	lua_pushcfunction(state, func); \
	lua_setglobal(state, #func);

REPLSystem::REPLSystem(EntityManager *em, ComponentManager *cm)
	: poll_fds { {.fd = STDIN_FILENO, .events = POLLIN} }
{
	_L = luaL_newstate();
	luaL_openlibs(_L);

	/* add global entitymanager and componentmanager */
	lua_pushlightuserdata(_L, em);
	lua_setglobal(_L, "entity_manager");

	lua_pushlightuserdata(_L, cm);
	lua_setglobal(_L, "component_manager");

	/* register functions */
	LUA_GLOBAL_FUNCTION(_L, entity_print);
	LUA_GLOBAL_FUNCTION(_L, entity_remove);
	LUA_GLOBAL_FUNCTION(_L, component_remove);
}

REPLSystem::~REPLSystem()
{
}

void REPLSystem::step(float dt)
{
	vector<string> lines = get_input();
	int error;

	for (string &line : lines) {
		error = luaL_loadbuffer(_L, line.c_str(), line.size(), "input");
		error |= lua_pcall(_L, 0, 0, 0);

		if (error) {
			printf("Error: %s\n", lua_tostring(_L, -1));
			lua_pop(_L, 1);
		}
	}
}

vector<string> REPLSystem::get_input()
{
	char cmd[256];
	vector<string> lines;

	if (poll(poll_fds, 1, 0) > 0 && (poll_fds[0].revents & POLLIN)) {
		while (fgets(cmd, 256, stdin) != 0)
			lines.push_back(string(cmd));
	}

	return lines;
}
