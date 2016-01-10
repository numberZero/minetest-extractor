#pragma once
#include "vfs.hxx"

class MyVFS: public VFS
{
private:
	MyVFS(PVirtualDirectory _root, FileName base, FileName minetest);

public:
	PVirtualDirectory root;
#define ADD_VIRTUAL(name)	PVirtualDirectory name
#define ADD_SANDBOX(name, path)	PSandboxDirectory name
#include "myvfs.inc"
#undef ADD_SANDBOX
#undef ADD_VIRTUAL
	
	MyVFS(FileName base);
	MyVFS(FileName base, FileName minetest);
};

extern MyVFS *fs;
