#pragma once
#include <list>
#include <map>
#include "iface.hxx"

class SandboxFile;
class SandboxDirectory;

typedef std::shared_ptr<SandboxFile> PSandboxFile;
typedef std::shared_ptr<SandboxDirectory> PSandboxDirectory;
typedef std::weak_ptr<SandboxFile> WPSandboxFile;
typedef std::weak_ptr<SandboxDirectory> WPSandboxDirectory;

class SandboxFile: public IFile
{
	FileName realpath;
	bool modified;
	WPSandboxDirectory parent;
	ByteArray buffer;

public:
	SandboxFile(PSandboxDirectory parent, FileName path);

	ByteArray read() override;
	void write(ByteArray const& data) override;
	
// 	void write(ByteArray&& data);
};

class SandboxDirectory: public IDirectory
{
	friend class SandboxFile;

	FileName realpath;
	std::list<PINode> data;
	std::map<FileName, WPINode> sandboxed;
	
public:
	SandboxDirectory(FileName path);

	PINode getEntry(FileName const& name) override;
	PIFile createFile(FileName const& name) override;
	PIDirectory createDirectory(FileName const& name) override;

	PINode addEntry(FileName const& name, PINode entry);
};
