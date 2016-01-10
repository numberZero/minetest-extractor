#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

typedef std::string FileName;
typedef std::vector<std::uint8_t> ByteArray;

class INode;
class IFile;
class IDirectory;

typedef std::shared_ptr<INode> PINode;
typedef std::shared_ptr<IFile> PIFile;
typedef std::shared_ptr<IDirectory> PIDirectory;
typedef std::weak_ptr<INode> WPINode;
typedef std::weak_ptr<IFile> WPIFile;
typedef std::weak_ptr<IDirectory> WPIDirectory;

class INode: public std::enable_shared_from_this<INode>
{
public:
	virtual ~INode();
};

class IFile: public INode
{
public:
	virtual ByteArray read() = 0;
	virtual void write(ByteArray const& data) = 0;
};

class IDirectory: public INode
{
public:
	virtual PINode getEntry(FileName const& name) = 0;
	virtual PIFile createFile(FileName const& name) = 0;
	virtual PIDirectory createDirectory(FileName const& name) = 0;
};
