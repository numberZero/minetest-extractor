#pragma once
#include <list>
#include <map>
#include <memory>
#include <string>
#include "mod.hxx"

class Modpack
{
	typedef std::map<std::string, std::unique_ptr<Modpack>> Index;
	static Index index;

	std::string name;
	std::string path;
//	std::list<Mod const*> contents;

	Modpack(std::string const& modpackname, std::string const& modpackpath);
public:
	std::string const& getname() const { return name; }
	std::string const& getpath() const { return path; }

	static Index& getindex() { return index; }
	static Modpack& get(std::string const& modpackname);
	static Modpack& add(std::string const& modpackname, std::string const& dirname);
	static void adddir(std::string const& dirname);
};
