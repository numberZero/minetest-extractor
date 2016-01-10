#pragma once
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>

bool fexists(std::string const& filename);
std::string readfile(std::string const& filename);

std::string basename(std::string path);
std::string dirname(std::string path);
std::string realpath(std::string path);

template <typename _OIter>
void listdir(std::string const& dirname, _OIter iter);

// Templates implementation

template <typename _OIter>
void listdir(std::string const& dirname, _OIter iter)
{
	DIR *dir = opendir(dirname.c_str());
	dirent *ent;
	while((ent = readdir(dir)))
		iter = std::string(ent->d_name);
	closedir(dir);
}
