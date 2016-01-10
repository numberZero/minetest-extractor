#include "virtual.hxx"
#include <stdexcept>

ByteArray VirtualFile::read()
{
	return contents;
}

void VirtualFile::write(const ByteArray& data)
{
	contents = data;
}

void VirtualFile::write(ByteArray&& data)
{
	contents = std::move(data);
}

PINode VirtualDirectory::getEntry(const FileName& name)
{
	auto iter = contents.find(name);
	PINode result;
	if(iter != contents.end())
		result = iter->second;
	return result;
}

PIFile VirtualDirectory::createFile(const FileName& name)
{
	PIFile file = std::make_shared<VirtualFile>();
	addEntry(name, file);
	return file;
}

PIDirectory VirtualDirectory::createDirectory(const FileName& name)
{
	PIDirectory dir = std::make_shared<VirtualDirectory>();
	addEntry(name, dir);
	return dir;
}

PINode VirtualDirectory::addEntry(FileName const& name, PINode entry)
{
	PINode& node = contents[name];
	if(node)
		throw std::runtime_error("Object exists: " + name);
	node = entry;
	return node;
}
