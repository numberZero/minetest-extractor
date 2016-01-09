cmake_minimum_required(VERSION 2.6)
project(extractor)

add_executable(extractor main.cpp)

install(TARGETS extractor RUNTIME DESTINATION bin)
