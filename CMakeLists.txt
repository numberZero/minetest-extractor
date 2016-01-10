cmake_minimum_required(VERSION 2.6)
project(extractor)

add_subdirectory(vfs)
add_subdirectory(lua)

find_package(Lua REQUIRED)

add_executable(extractor
	main.cxx
	common/func.cxx
	mt/mod.cxx
	mt/modpack.cxx
	${VFS_SOURCE}
	${LUA_SOURCE}
)

target_include_directories(extractor PUBLIC
	${CMAKE_SOURCE_DIR}
	${LUA_INCLUDE_DIRS}
)

target_link_libraries(extractor
	${LUA_LIBRARIES}
)

target_compile_features(extractor PUBLIC
	cxx_nullptr
	cxx_range_for
)

install(TARGETS extractor RUNTIME DESTINATION bin)
