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

std::vector< unsigned char > readfile(const std::string& filename)
{
	FILE *file = fopen(filename.c_str(), "rb");
	if(!file)
		return{};
	std::vector<unsigned char> data;
	fseek(file, 0, SEEK_END);
	data.resize(ftell(file));
	fseek(file, 0, SEEK_SET);
	unsigned char* pdata = data.data();
	long rem = data.size();
	for(;;)
	{
		long read = fread(pdata, 1, rem, file);
		if(!read)
			break;
		pdata += read;
		rem -= read;
	}
	fclose(file);
	data.resize(pdata - data.data());
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
