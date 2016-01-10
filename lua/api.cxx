#include "api.hxx"
#include <iostream>
#include <new>
#include <sstream>
#include "vfs/myvfs.hxx"

int luasafe_log(lua_State *L)
{
	int argcount = lua_gettop(L);
	for(int i = 0; i != argcount; ++i)
	{
		char const* arg = lua_tolstring(L, i + 1, nullptr);
		if(i)
			std::clog << "\t";
		std::clog << arg;
	}
	std::clog << std::endl;
	lua_pop(L, argcount);
	return 0;
}

int luasafe_dofile(lua_State* L)
{
	int argcount = lua_gettop(L);
	if(argcount != 1)
	{
		lua_pop(L, argcount);
		return 0;
	}
	char const* path = lua_tolstring(L, 1, nullptr);
	ByteArray data = fs->read(FileName(path));
	lua_pop(L, lua_gettop(L));
	luaL_loadbuffer(L, reinterpret_cast<char const*>(data.data()), data.size(), path);
	lua_call(L, 0, LUA_MULTRET);
	return lua_gettop(L);
}

extern cookie_io_functions_t iostr_cookie;

struct mystream: luaL_Stream
{
	std::stringstream data;
	PIFile file;
	bool read = false;
	bool write = false;
	bool create = false;
	
	mystream(FileName path, std::string mode)
	{
		bool erase = false;
		closef = luaint_io_close;
		for(char c: mode)
			switch(c)
			{
				case 'r':
					read = true;
					break;
				case 'w':
					write = true;
					erase = true;
					create = true;
					break;
				case '+':
					read = true;
					write = true;
					create = true;
					break;
				case 'b':
					break;
				default:
					throw std::runtime_error("Invalid mode supplied");
			}
		PINode node = fs->get(path);
		if(!node && create)
		{
			std::clog << "Creating file " << path << std::endl;
			node = fs->createFile(path);
		}
		file = std::dynamic_pointer_cast<IFile>(node);
		if(!file)
			throw 0;
		std::clog << "Opened file " << path << std::endl;
		if(erase)
			file->write(ByteArray{});
		data.str(file->read());
		data.seekp(0);
		f = fopencookie(static_cast<std::iostream*>(&data), mode.c_str(), iostr_cookie);
	}
	
	~mystream()
	{
		file->write(data.str());
	}
};

int luasafe_io_open(lua_State *L)
{
	int args = lua_gettop(L);
	FileName path;
	std::string mode;
	switch(args)
	{
		case 2:
			mode = lua_tostring(L, 2);
			break;
		case 1:
			mode = "r";
			break;
		default:
			return 0;
	}
	path = lua_tostring(L, 1);
	std::clog << "io.open: " << path << " (" << mode << ")" << std::endl;
	lua_pop(L, args);
	mystream *str = reinterpret_cast<mystream*>(lua_newuserdata(L, sizeof(mystream)));
	try
	{
		new(str) mystream(path, mode);
	}
	catch(...)
	{
		lua_pop(L, 1);
		lua_pushnil(L);
		lua_pushstring(L, ("Can't open the file: " + path).c_str());
		return 2;
	}
 	luaL_setmetatable(L, LUA_FILEHANDLE);
	return 1;
}

int luaint_io_close(lua_State *L)
{
	if(lua_gettop(L) != 1)
		return 0;
	mystream *str = reinterpret_cast<mystream*>(lua_touserdata(L, 1));
	str->~mystream();
	lua_pop(L, 1);
	return 0;
}

ssize_t iostr_read(void *cookie, char *buf, size_t size)
{
	std::iostream* str = reinterpret_cast<std::iostream*>(cookie);
	long pos1 = str->tellg();
	str->read(buf, size);
	long pos2 = str->tellg();
	str->seekp(pos2);
	return pos2 - pos1;
}

ssize_t iostr_write(void *cookie, const char *buf, size_t size)
{
	std::iostream* str = reinterpret_cast<std::iostream*>(cookie);
	long pos1 = str->tellp();
	str->write(buf, size);
	long pos2 = str->tellp();
	str->seekg(pos2);
	return pos2 - pos1;
}

int iostr_seek(void *cookie, off64_t *offset, int whence)
{
	std::iostream* str = reinterpret_cast<std::iostream*>(cookie);
	switch(whence)
	{
		case SEEK_SET:
			str->seekg(*offset, std::ios_base::beg);
			break;
		case SEEK_CUR:
			str->seekg(*offset, std::ios_base::cur);
			break;
		case SEEK_END:
			str->seekg(*offset, std::ios_base::end);
			break;
	}
	*offset = str->tellg();
	str->seekp(*offset);
	return 0;
}

cookie_io_functions_t iostr_cookie = {
	iostr_read,
	iostr_write,
	iostr_seek,
	nullptr
};

//int luasafe_io_lines(lua_State *L)
/*
int luasafe_file_read(lua_State *L)
{
}

int luasafe_file_write(lua_State *L)
{
}*/
