#include "sandbox.hxx"
#include "common/func.hxx"
#include "virtual.hxx"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

SandboxFile::SandboxFile(PSandboxDirectory _parent, FileName path):
	realpath(path),
	modified(false),
	parent(_parent)
{
}

ByteArray SandboxFile::read()
{
	if(modified)
		return buffer;
	return std::move(readfile(realpath));
}

void SandboxFile::write(const ByteArray& data)
{
	if(!modified)
	{
		PSandboxDirectory dir = parent.lock();
		if(dir)
			dir->data.push_back(std::static_pointer_cast<INode>(shared_from_this()));
	}
	modified = true;
	buffer = data;
}

// void SandboxFile::write(ByteArray&& data)
// {
// 	modified = true;
// 	buffer = std::move(data);
// }

SandboxDirectory::SandboxDirectory(FileName path) :
	realpath(path)
{
}

PINode SandboxDirectory::getEntry(const FileName& name)
{
	auto iter = sandboxed.find(name);
	PINode node;
	if(iter != sandboxed.end())
		node = iter->second.lock();
	if(node)
		return node;
	std::string path = realpath + "/" + name;
	struct stat st;
	stat(path.c_str(), &st);
	if(S_ISREG(st.st_mode))
	{
		node = std::make_shared<SandboxFile>(std::static_pointer_cast<SandboxDirectory>(shared_from_this()), path);
	}
	else if(S_ISDIR(st.st_mode))
	{
		node = std::make_shared<SandboxDirectory>(path);
		data.push_back(node);
	}
	else
	{
		return node;
	}
	sandboxed[name] = WPINode(node);
	return node;
}

PIFile SandboxDirectory::createFile(const FileName& name)
{
	PIFile file = std::make_shared<VirtualFile>();
	addEntry(name, file);
	return file;
}

PIDirectory SandboxDirectory::createDirectory(const FileName& name)
{
	PIDirectory dir = std::make_shared<VirtualDirectory>();
	addEntry(name, dir);
	return dir;
}

PINode SandboxDirectory::addEntry(FileName const& name, PINode entry)
{
	if(getEntry(name))
		throw std::runtime_error("Object exists: " + name);
	data.push_back(entry);
	sandboxed[name] = WPINode(entry);
	return entry;
}
