#include "mod.hxx"
#include <fstream>
#include <iostream>
#include <list>
#include <stdexcept>
#include "common/func.hxx"
#include "vfs/myvfs.hxx"

#define THROW_NOTIMPL	throw std::logic_error("Method not implemented: " + std::string(__PRETTY_FUNCTION__))

std::map<std::string, std::unique_ptr<Mod>> Mod::index;

// Mod& ModDependency::get_mod()
// {
// 	if(!mod)
// 		mod = &Mod::get(modname);
// 	return *mod;
// }

Mod::Mod(std::string const& modname, std::string const& modpath) :
	name(modname),
	path(modpath)
{
	std::clog << "mod " << modname << std::endl;
	std::ifstream depends_file(modpath + "/depends.txt");
	if(depends_file)
	{
		for(;;)
		{
			bool required = true;
			std::string depend_line;
			depends_file >> depend_line;
			if(depend_line.empty())
				break;
			if(depend_line[depend_line.size() - 1] == '?') // optional dependency
			{
				required = false;
				depend_line.resize(depend_line.size() - 1); // strip the question mark
			}
			std::clog << " - depends: " << depend_line;
			if(required)
				std::clog << " (required)";
			else
				std::clog << " (optional)";
			std::clog << std::endl;
			depends.push_back(ModDependency{depend_line, required});//, nullptr});
		}
	}
	else
		std::clog << "Can't find the dependency file at " << modpath + "/depends.txt" << std::endl;
	PSandboxDirectory dir = std::make_shared<SandboxDirectory>(path);
	fs->mod->addEntry(name, dir);
}

void Mod::real_load(lua_State* L)
{
 	std::clog << "Running " << name << std::endl;
	state = ModState::Running;
	lua_getglobal(L, "__builtin__load_mod");
	lua_pushstring(L, name.c_str());
	int err = lua_pcall(L, 1, 1, 0);
	if(err)
	{
		std::size_t len;
		char const* pch = lua_tolstring(L, -1, &len);
		std::string str(pch, len);
		lua_pop(L, 1); // pch is no longer valid
		state = ModState::Error;
		throw std::runtime_error("Error during loading mod " + name + ": " + str);
	}
	lua_pop(L, 1);
	state = ModState::Ready;
}

void Mod::load(lua_State* L)
{
	std::clog << "Loading " << name << std::endl;
	switch(state)
	{
		case ModState::Indexed:
			break;
		case ModState::Ready:
			std::clog << "Mod " + name + " is already loaded" << std::endl;
			return;
		case ModState::Loading:
			throw std::runtime_error("Cyclic dependency detected in mod " + name);
		default:
			throw std::runtime_error("Mod " + name + " is not in a correct state");
	}
	state = ModState::Loading;
	for(ModDependency const& dep: depends)
	{
		auto pmod = index.find(dep.modname);
		if(pmod != index.end())
			pmod->second->load(L);
		else if(dep.required)
		{
			state = ModState::Indexed;
			throw std::runtime_error("Mod " + name + " has unsatisfied dependency: " + dep.modname);
		}
		else
			std::clog << "Mod " + name + " has unsatisfied optional dependency: " + dep.modname << std::endl;
	}
	real_load(L);
	std::clog << "Mod " + name + " is ready" << std::endl;
}

Mod& Mod::get(std::string const& modname)
{
	return *index.at(modname).get();
}

Mod& Mod::add(std::string const& modname, std::string const& dirname)
{
	if(index.find(modname) != index.end())
		throw std::runtime_error("Mod conflict: " + modname);
	std::clog << "adding mod " << modname << " from " << dirname << std::endl;
	std::unique_ptr<Mod> pmod(new Mod(modname, dirname));
	Mod& mod = *index.emplace(modname, std::move(pmod)).first->second;
	mod.state = ModState::Indexed;
	return mod;
}

void Mod::adddir(std::string const& dirname)
{
	std::list<std::string> dirs;
	listdir(dirname, std::back_inserter(dirs));
	for(std::string const& name: dirs)
	{
		if(name[0] == '.')
			continue;
		std::string path = dirname + "/" + name;
		if(fexists(path + "/init.lua"))
			add(name, path);
	}
}

void Mod::loadall(lua_State* L)
{
	for(auto const& ref: index)
		ref.second->load(L);
}
