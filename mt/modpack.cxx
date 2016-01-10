#include "modpack.hxx"
#include <fstream>
#include <iostream>
#include <list>
#include <stdexcept>
#include "common/func.hxx"
#include "vfs/myvfs.hxx"

#define THROW_NOTIMPL	throw std::logic_error("Method not implemented: " + std::string(__PRETTY_FUNCTION__))

std::map<std::string, std::unique_ptr<Modpack>> Modpack::index;

// Modpack& ModpackDependency::get_modpack()
// {
// 	if(!modpack)
// 		modpack = &Modpack::get(modpackname);
// 	return *modpack;
// }

Modpack::Modpack(std::string const& modpackname, std::string const& modpackpath) :
	name(modpackname),
	path(modpackpath)
{
	std::clog << "modpack " << modpackname << std::endl;
	Mod::adddir(path);
}

Modpack& Modpack::get(std::string const& modpackname)
{
	return *index.at(modpackname).get();
}

Modpack& Modpack::add(std::string const& modpackname, std::string const& dirname)
{
	if(index.find(modpackname) != index.end())
		throw std::runtime_error("Modpack conflict: " + modpackname);
	std::clog << "adding modpack " << modpackname << " from " << dirname << std::endl;
	std::unique_ptr<Modpack> pmodpack(new Modpack(modpackname, dirname));
	Modpack& modpack = *index.emplace(modpackname, std::move(pmodpack)).first->second;
	return modpack;
}

void Modpack::adddir(std::string const& dirname)
{
	std::list<std::string> dirs;
	listdir(dirname, std::back_inserter(dirs));
	for(std::string const& name: dirs)
	{
		if(name[0] == '.')
			continue;
		std::string path = dirname + "/" + name;
		if(fexists(path + "/modpack.txt"))
			add(name, path);
	}
}
