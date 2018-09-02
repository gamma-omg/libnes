cmake_minimum_required(VERSION 3.12)
project(nescore-tests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../src)

add_executable(test_cpu src/TestOfficialInstructions.cpp src/TestUnofficialInstructions.cpp)
add_executable(test_memory src/TestCPUMemory.cpp)
add_executable(test_rom src/TestRom.cpp)
add_executable(test_programs src/TestPrograms.cpp src/utils/TestProgram.cpp src/utils/TestProgram.h)
add_executable(test_renderer src/TestRenderer.cpp)
add_executable(test_nescore src/TestOfficialInstructions.cpp src/TestCPUMemory.cpp src/TestRom.cpp src/TestPrograms.cpp src/TestUnofficialInstructions.cpp src/utils/TestProgram.cpp src/utils/TestProgram.h src/TestRenderer.cpp)

target_link_libraries(test_cpu gtest gtest_main nescore)
target_link_libraries(test_memory gtest gtest_main nescore)
target_link_libraries(test_rom gtest gtest_main nescore)
target_link_libraries(test_programs gtest gtest_main nescore)
target_link_libraries(test_renderer gtest gtest_main nescore)
target_link_libraries(test_nescore gtest gtest_main nescore)
