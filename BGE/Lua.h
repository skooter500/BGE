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
	static void RegisterMembers(lua_State* l);
};