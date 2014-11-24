#pragma once

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge.h>

class Lua
{
public:
	static void registerMembers(lua_State* l);
};