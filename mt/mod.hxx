#pragma once
#include <list>
#include <map>
#include <memory>
#include <string>
#include <lua.hpp>

class Mod;

struct ModDependency
{
	std::string modname;
	bool required;
//	Mod* mod;
	
//	Mod& get_mod(); // throws if mod doesn't present in the index
};

enum class ModState
{
	Unknown,
	Error,
	Indexed,
	Loading,
	Running,
	Ready
};

class Mod
{
	typedef std::map<std::string, std::unique_ptr<Mod>> Index;
	static Index index;
	
	ModState state = ModState::Unknown;
	std::string name;
	std::string path;
	std::list<ModDependency> depends;
	void real_load(lua_State *L);
	
	Mod(std::string const& modname, std::string const& modpath);
public:
	void load(lua_State *L);
	
	std::string const& getname() const { return name; }
	std::string const& getpath() const { return path; }

	static Index& getindex() { return index; }
	static Mod& get(std::string const& modname);
	static Mod& add(std::string const& modname, std::string const& dirname);
	static void adddir(std::string const& dirname);
 	static void loadall(lua_State *L);
};
