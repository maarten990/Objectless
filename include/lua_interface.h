#ifndef LUA_INTERFACE_H
#define LUA_INTERFACE_H

#include <lua.hpp>
#include <stdio.h>
#include "Engine/EntityManager.h"
#include "Engine/ComponentManager.h"

int entity_print(lua_State *L)
{
	int entity = luaL_checkinteger(L, -1);

	lua_getglobal(L, "entity_manager");
	EntityManager *em = (EntityManager *)lua_touserdata(L, -1);

	printf("%s\n", em->pretty(entity).c_str());

	return 0;
}

int entity_remove(lua_State *L)
{
	int entity = luaL_checkinteger(L, -1);

	lua_getglobal(L, "entity_manager");
	EntityManager *em = (EntityManager *)lua_touserdata(L, -1);

	em->remove(entity);
	printf("Removed entity %d\n", entity);

	return 0;
}

/* component_remove(entity, component_name)
 * Note that the last argument will be on top of the stack. */
int component_remove(lua_State *L)
{
	int entity = luaL_checkint(L, -2);
	const char *component = luaL_checkstring(L, -1);

	lua_getglobal(L, "entity_manager");
	EntityManager *em = (EntityManager *)lua_touserdata(L, -1);

	lua_getglobal(L, "component_manager");
	ComponentManager *cm = (ComponentManager *)lua_touserdata(L, -1);

	printf("Removed component %s from entity %d\n", component, entity);
	em->remove_component(entity, *cm->index_from_name(component));

	return 0;
}

#endif
