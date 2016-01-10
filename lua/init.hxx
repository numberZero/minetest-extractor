#pragma once
#include <lua.hpp>

class Lua
{
private:
	lua_State *state;
	
public:
	Lua();
	~Lua();

	operator lua_State* () { return state; }
};

extern Lua lua;
