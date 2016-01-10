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

std::map<std::string, std::string> mods;

void addmod(std::string const& name, std::string const& path)
{
	if(mods.find(name) != mods.end())
		throw std::runtime_error("Mod conflict: " + name);
	std::clog << "mod found: " << name << " at " << path << std::endl;
	mods[name] = path;
}

void addmods(std::string const& dir)
{
	std::list<std::string> dirs;
	listdir(dir, std::back_inserter(dirs));
	for(std::string const& name: dirs)
	{
		if(name[0] == '.')
			continue;
		std::string path = dir + "/" + name;
		if(fexists(path + "/init.lua"))
			addmod(name, path);
	}
}

void addmods2(std::string const& root)
{
	std::list<std::string> dirs;
	listdir(root, std::back_inserter(dirs));
	for(std::string const& name: dirs)
	{
		if(name[0] == '.')
			continue;
		std::string path = root + "/" + name;
		if(fexists(path + "/init.lua"))
			addmod(name, path);
		else if(fexists(path + "/modpack.txt"))
		{
			std::clog << "-- modpack found: " << name << std::endl;
			addmods(path);
			std::clog << "-- modpack end" << std::endl;
		}
	}
}

void indexMods()
{
	PVirtualDirectory dir_mod = std::dynamic_pointer_cast<VirtualDirectory>(fs->get("/mod"));
	Mod::adddir(GAMEROOT + "/mods");
	Mod::adddir(MODSROOT);
	for(auto const& pmod: Mod::getindex())
	{
		PSandboxDirectory dir = std::make_shared<SandboxDirectory>(pmod.second->getpath());
		dir_mod->addEntry(pmod.second->getname(), dir);
	}
}

int main(int argc, char **argv)
{
	FileName self(argv[0]);
	FileName base(realpath(dirname(self) + "/.."));
	L = lua;
	fs = new MyVFS(base, realpath(base + "/../base-minetest"));
	indexMods();
	runlua("main.lua");
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
	delete fs;
	return 0;
}
