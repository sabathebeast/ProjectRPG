#include "luaVM.h"

#ifdef _WIN64
#pragma comment(lib, "LUA/liblua54.a")
#endif // _WIN64

LuaVM::LuaVM()
{
	luaStatePtr = luaL_newstate();
	if (luaStatePtr)
	{
		luaL_openlibs(luaStatePtr);

		if (checkLua(luaStatePtr, luaL_loadfile(luaStatePtr, "./player.lua")))
		{
			if (checkLua(luaStatePtr, lua_pcall(luaStatePtr, 0, LUA_MULTRET, 0)))
			{
				isLuaOk = true;
			}
		}
	}
}

LuaVM::~LuaVM()
{
	if (luaStatePtr) lua_close(luaStatePtr);
}

bool LuaVM::checkLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}
	return true;
}
