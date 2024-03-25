#pragma once

#include "LUA/include/lua.hpp"
#include <iostream>

class LuaVM
{
public:
	LuaVM();
	~LuaVM();

private:
	lua_State* luaStatePtr = nullptr;
	bool checkLua(lua_State* L, int r);
	bool isLuaOk = false;
};