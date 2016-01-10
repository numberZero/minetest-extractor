#pragma once
#include <map>
#include "iface.hxx"

class VirtualFile: public IFile
{
	ByteArray contents;

public:
	ByteArray read() override;
	void write(ByteArray const& data) override;

	void write(ByteArray&& data);
};

class VirtualDirectory: public IDirectory
{
	std::map<FileName, PINode> contents;
	
public:
	PINode getEntry(FileName const& name) override;
	PIFile createFile(FileName const& name) override;
	PIDirectory createDirectory(FileName const& name) override;

	PINode addEntry(FileName const& name, PINode entry);
};

typedef std::shared_ptr<VirtualFile> PVirtualFile;
typedef std::shared_ptr<VirtualDirectory> PVirtualDirectory;
typedef std::weak_ptr<VirtualFile> WPVirtualFile;
typedef std::weak_ptr<VirtualDirectory> WPVirtualDirectory;
