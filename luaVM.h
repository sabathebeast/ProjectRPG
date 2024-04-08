#pragma once

#include "LUA/include/lua.hpp"
#include <iostream>
#include <typeinfo>

class LuaVM
{
public:
	LuaVM();
	~LuaVM();

	inline const bool getLuaState() const { return isLuaOk; }

	template<typename T, typename... Args>
	T callLuaFunction(const std::string& functionName, Args... args) {
		lua_getglobal(luaStatePtr, functionName.c_str());

		lua_Debug ar;
		lua_pushvalue(luaStatePtr, -1);
		lua_getinfo(luaStatePtr, ">u", &ar);
		int expectedArgs = ar.nparams;
		lua_pop(luaStatePtr, 1);

		lua_getglobal(luaStatePtr, functionName.c_str());
		pushArgsToStack(args...);
		int pushedArgs = sizeof...(args);

		if (expectedArgs == pushedArgs)
		{
			if (checkLua(luaStatePtr, lua_pcall(luaStatePtr, pushedArgs, 1, 0)))
			{
				T result = popResultFromStack<T>(luaStatePtr);
				lua_pop(luaStatePtr, 1);
				return result;
			}
		}
		else
		{
			std::cerr << "FAIL on " << "'" << functionName << "'" << " expected " << expectedArgs << " arguments" << " and got " << pushedArgs << std::endl;
			return T();
		}
	}

	template <typename T>
	T getLuaVariable(const std::string& varName) {
		lua_getglobal(luaStatePtr, varName.c_str());
		T result = popResultFromStack<T>(luaStatePtr);
		lua_pop(luaStatePtr, 1);
		return result;
	}

	template <typename T>
	void setLuaVariable(const std::string& varName, T value) {
		pushValueToStack(value);
		lua_setglobal(luaStatePtr, varName.c_str());
	}

private:
	lua_State* luaStatePtr = nullptr;
	bool checkLua(lua_State* L, int r);
	bool isLuaOk = false;

	template <typename T>
	void pushValueToStack(T value) {
		if (luaStatePtr)
		{
			if constexpr (std::is_integral_v<T>) {
				if constexpr (std::is_same_v<T, bool>) {
					lua_pushboolean(luaStatePtr, value);
				}
				else
				{
					lua_pushinteger(luaStatePtr, value);
				}
			}
			else if constexpr (std::is_floating_point_v<T>) {
				lua_pushnumber(luaStatePtr, value);
			}
			else if constexpr (std::is_same_v<T, const char*>) {
				lua_pushstring(luaStatePtr, value);
			}
			else if constexpr (std::is_same_v<T, std::string>) {
				lua_pushstring(luaStatePtr, value.c_str());
			}
			else
			{
				std::cerr << "Error pushing value to stack: " << lua_tostring(luaStatePtr, -1) << " type: " << typeid(T).name() << std::endl;
				return 1;
			}
		}
	}

	template <typename T, typename... Rest>
	void pushArgsToStack(T arg, Rest... rest) {
		pushValueToStack(arg);
		pushArgsToStack(rest...);
	}

	void pushArgsToStack() {}

	template <typename T>
	T popResultFromStack(lua_State* L) {
		if constexpr (std::is_integral_v<T>) {
			if (!lua_isinteger(L, -1)) {
				std::cerr << "Lua return value is not an integer, it actually is a: " << typeid(T).name() << std::endl;
				return T();
			}
			return (T)lua_tointeger(L, -1);
		}
		else if constexpr (std::is_floating_point_v<T>) {
			if (!lua_isnumber(L, -1)) {
				std::cerr << "Lua return value is not a float, it actually is a: " << typeid(T).name() << std::endl;
				return T();
			}
			return (T)lua_tonumber(L, -1);
		}
		else if constexpr (std::is_same_v<T, const char*>) {
			if (!lua_isstring(L, -1)) {
				std::cerr << "Lua return value is not a char*, it actually is a: " << typeid(T).name() << std::endl;
				return T();
			}
			return (T)lua_tostring(L, -1);
		}
		else if constexpr (std::is_same_v<T, bool>) {
			if (!lua_isboolean(L, -1)) {
				std::cerr << "Lua return value is not a boolean, it actually is a: " << typeid(T).name() << std::endl;
				return T();
			}
			return (T)lua_toboolean(L, -1);
		}
		else if constexpr (std::is_same_v<T, std::string>) {
			if (!lua_isstring(L, -1)) {
				std::cerr << "Lua return value is not a string, it actually is a: " << typeid(T).name() << std::endl;
				return T();
			}
			return std::string(lua_tostring(L, -1));
		}
		else
		{
			std::cerr << "Error popping result from stack: " << lua_tostring(L, -1) << " type: " << typeid(T).name() << std::endl;
			return 1;
		}
	}
};