#include "myvfs.hxx"

MyVFS *fs;

MyVFS::MyVFS(PVirtualDirectory _root, FileName base, FileName minetest):
	VFS(_root),
	root(_root)
{
#define ADD(name)	root->addEntry(#name, name);
#define ADD_VIRTUAL(name)	name = std::make_shared<VirtualDirectory>(); ADD(name)
#define ADD_SANDBOX(name, path)	name = std::make_shared<SandboxDirectory>(path); ADD(name)
#include "myvfs.inc"
#undef ADD_SANDBOX
#undef ADD_VIRTUAL
#undef ADD
}

MyVFS::MyVFS(FileName base, FileName minetest) :
	MyVFS(std::make_shared<VirtualDirectory>(), base, minetest)
{
}

MyVFS::MyVFS(FileName base) :
	MyVFS(base, base)
{
}
