cmake_minimum_required(VERSION 3.12)
project(nescore)

add_subdirectory(libs/gtest)
add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES src/CPU.h src/CPU.cpp src/Memory.cpp src/Memory.h)
add_library(nescore ${SOURCE_FILES})
