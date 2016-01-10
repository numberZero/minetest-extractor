#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <lua.hpp>
#include "common/func.hxx"
#include "lua/init.hxx"
#include "mt/mod.hxx"
#include "mt/modpack.hxx"
#include "vfs/myvfs.hxx"

#define LUA_REGISTER_CONST(name) lua_pushstring(L, name.c_str()); lua_setglobal(L, #name)

static std::string const WORLDROOT = "world";
static std::string const MODSROOT = "../mods";
static std::string const GAMEROOT = "../games/minetest_game";

lua_State *L;

void runlua(std::string const& filename)
{
	std::clog << "LUA chunk load result: " << luaL_dofile(L, filename.c_str()) << std::endl;
	int top;
	while((top = lua_gettop(L)))
	{
		std::clog << lua_tolstring(L, top, 0) << std::endl;
		lua_pop(L, 1);
	}
}

void indexMods()
{
	Mod::adddir(GAMEROOT + "/mods");
	Modpack::adddir(GAMEROOT + "/mods");
	Mod::adddir(MODSROOT);
	Modpack::adddir(MODSROOT);
}

int main(int argc, char **argv)
{
	FileName self(argv[0]);
	FileName base(realpath(dirname(self) + "/.."));
	L = lua;
	fs = new MyVFS(base, realpath(base + "/../base-minetest"));
	indexMods();
	runlua("script/init.lua");
	try
	{
		std::clog << "=== Loading mods ===" << std::endl;
		Mod::loadall(L);
		std::clog << "=== Loading completed ===" << std::endl;
	}
	catch(std::exception const& e)
	{
		std::clog << "Exception caught: " << e.what() << std::endl;
	}
	catch(...)
	{
		std::clog << "Unknown exception caught" << std::endl;
	}
	runlua("script/main.lua");
	delete fs;
	return 0;
}
