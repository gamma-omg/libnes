cmake_minimum_required(VERSION 3.12)
project(nescore-tests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../src)

add_executable(test_cpu cpu/TestInstructions.cpp cpu/TestPrograms.cpp)
add_executable(test_memory memory/TestMemory.cpp)
add_executable(test_rom rom/TestRom.cpp)
add_executable(test_nescore cpu/TestInstructions.cpp memory/TestMemory.cpp rom/TestRom.cpp cpu/TestPrograms.cpp)

target_link_libraries(test_cpu gtest gtest_main)
target_link_libraries(test_memory gtest gtest_main)
target_link_libraries(test_rom gtest gtest_main)
target_link_libraries(test_nescore gtest gtest_main)

target_link_libraries(test_cpu nescore)
target_link_libraries(test_memory nescore)
target_link_libraries(test_rom nescore)
target_link_libraries(test_nescore nescore)