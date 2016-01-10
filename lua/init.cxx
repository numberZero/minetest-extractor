#include "init.hxx"
#include "api.hxx"

Lua lua;

Lua::Lua()
{
	state = luaL_newstate();
	luaopen_base(state);
	lua_pop(state, 1);
	luaopen_string(state);
	lua_setglobal(state, "string");
	luaopen_table(state);
	lua_setglobal(state, "table");
	luaopen_math(state);
	lua_setglobal(state, "math");
	luaopen_io(state);
	lua_setglobal(state, "io");

// core
	lua_createtable(state, 0, 0);
	lua_pushcfunction(state, luasafe_log);
	lua_setfield(state, 1, "log");
	lua_setglobal(state, "core");

// _G
	lua_register(state, "__io_open", luasafe_io_open);
	lua_register(state, "dofile", luasafe_dofile);
}

Lua::~Lua()
{
	lua_close(state);
}
