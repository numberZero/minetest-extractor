#include "api.hxx"
#include <iostream>
#include "vfs/myvfs.hxx"

int luasafe_log(lua_State *L)
{
	int argcount = lua_gettop(L);
	for(int i = 0; i != argcount; ++i)
	{
		char const* arg = lua_tolstring(L, i + 1, nullptr);
		if(i)
			std::clog << "\t";
		std::clog << arg;
	}
	std::clog << std::endl;
	lua_pop(L, argcount);
	return 0;
}

int luasafe_dofile(lua_State* L)
{
	int argcount = lua_gettop(L);
	if(argcount != 1)
	{
		lua_pop(L, argcount);
		return 0;
	}
	char const* path = lua_tolstring(L, 1, nullptr);
	ByteArray data = fs->read(FileName(path));
	luaL_loadbuffer(L, reinterpret_cast<char const*>(data.data()), data.size(), path);
	lua_call(L, 0, 0);
	return 0;
}

int luasafe_io_open(lua_State *L)
{
	return 0;
}

int luaint_io_close(lua_State *L)
{
	return 0;
}

//int luasafe_io_lines(lua_State *L)
/*
int luasafe_file_read(lua_State *L)
{
}

int luasafe_file_write(lua_State *L)
{
}*/
