#pragma once
#include <memory>
#include "iface.hxx"
#include "virtual.hxx"
#include "sandbox.hxx"

class VFS
{
private:
	PIDirectory root;
	
public:
	VFS(PIDirectory _root);

	PINode get(FileName path);
	ByteArray read(FileName path);
	void write(FileName path, ByteArray const& data, bool canCreate = true);
	PIFile createFile(FileName path);
	PIDirectory createDirectory(FileName path);
};
