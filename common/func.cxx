#include "func.hxx"
#include <cstdio>

bool fexists(std::string const& filename)
{
	FILE *test = fopen(filename.c_str(), "rb");
	if(!test)
		return false;
	fclose(test);
	return true;
}

std::string readfile(const std::string& filename)
{
	FILE *file = fopen(filename.c_str(), "rb");
	if(!file)
		throw 0;
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	char* buffer = new char[len];
	fseek(file, 0, SEEK_SET);
	char* pdata = buffer;
	long rem = len;
	for(;;)
	{
		long read = fread(pdata, 1, rem, file);
		if(!read)
			break;
		pdata += read;
		rem -= read;
	}
	fclose(file);
	std::string data(buffer, pdata - buffer);
	delete[] buffer;
	return std::move(data);
}

std::string basename(std::string path)
{
	std::size_t pos = path.rfind('/');
	if(pos != path.npos)
		path = path.substr(pos + 1);
	return path;
}

std::string dirname(std::string path)
{
	std::size_t pos = path.rfind('/');
	if(pos != path.npos)
		path = path.substr(0, pos);
	return path;
}

std::string realpath(std::string path)
{
	char *buffer = realpath(path.c_str(), nullptr);
	if(buffer)
	{
		path = buffer;
		free(buffer);
	}
	else
		path.clear();
	return path;
}
