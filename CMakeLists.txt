cmake_minimum_required(VERSION 3.12)
project(nescore)

add_subdirectory(libs/gtest)
add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES src/CPU.h src/CPU.cpp src/Memory.cpp src/Memory.h src/access/ZP.h src/access/IMM.h src/access/ACC.h src/access/ZPX.h src/access/ZPY.h src/access/ABS.h src/access/ABS.h src/access/ABSX.h src/access/ABSY.h src/access/INDX.h src/access/INDY.h src/access/IMPL.h src/rom/INESRom.cpp src/rom/INESRom.h)
add_library(nescore ${SOURCE_FILES})
