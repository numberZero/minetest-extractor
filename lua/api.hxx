#pragma once
#include <lua.hpp>

int luasafe_log(lua_State *L);
int luasafe_dofile(lua_State *L);

int luasafe_io_open(lua_State *L);
int luaint_io_close(lua_State *L);
