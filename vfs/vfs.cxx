#include "vfs.hxx"
#include <stdexcept>

INode::~INode()
{
}

VFS::VFS(PIDirectory _root) :
	root(_root)
{
}

PINode VFS::get(FileName path)
{
	if(path[0] != '/')
		throw std::runtime_error("Only absolute paths are supported: " + path);
	std::list<PIDirectory> stack;
	stack.push_back(root);
	std::size_t pos = 1;
	std::size_t pos2;
	std::size_t len;
	FileName name;
	do
	{
		pos2 = path.find('/', pos);
		if(pos2 == FileName::npos)
			break;
		len = pos2 - pos;
		name = path.substr(pos, len);
		pos = pos2 + 1;
		if(name == "")
			continue;
		if(name == ".")
			continue;
		if(name == "..")
		{
			stack.pop_back();
			continue;
		}
		PIDirectory dir(std::dynamic_pointer_cast<IDirectory>(stack.back()->getEntry(name)));
		if(!dir)
			throw std::runtime_error("Directory not found at " + path);
		stack.push_back(dir);
	} while(pos2 != FileName::npos);
	name = path.substr(pos);
	return stack.back()->getEntry(name);
}

ByteArray VFS::read(FileName path)
{
	PINode node = get(path);
	IFile* file = dynamic_cast<IFile*>(node.get());
	if(!file)
		throw std::runtime_error("No file at " + path);
	return file->read();
}

void VFS::write(FileName path, const ByteArray& data, bool canCreate)
{
	PINode node = get(path);
	IFile* file;
	if(!node)
	{
		if(canCreate)
			file = createFile(path).get();
		else
			throw std::runtime_error("No file at " + path);
	}
	else
		file = dynamic_cast<IFile*>(node.get());
	if(!file)
		throw std::runtime_error("No file at " + path);
	file->write(data);
}

PIFile VFS::createFile(FileName path)
{
	std::size_t sep = path.rfind('/');
	PIDirectory dir = std::dynamic_pointer_cast<IDirectory>(get(path.substr(0, sep)));
	return dir->createFile(path.substr(sep + 1));
}

PIDirectory VFS::createDirectory(FileName path)
{
	std::size_t sep = path.rfind('/');
	PIDirectory dir = std::dynamic_pointer_cast<IDirectory>(get(path.substr(0, sep)));
	return dir->createDirectory(path.substr(sep + 1));
}
