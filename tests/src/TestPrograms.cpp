#include <gtest/gtest.h>
#include <fstream>
#include <rom/INESRom.h>
#include <mappers/MapperFactory.h>
#include <CPU.h>
#include <Memory.h>
#include "utils/TestProgram.h"

using namespace nescore;

TEST(CPU, immediate)
{
    TestProgram test("tests/data/02-immediate.nes");

    try
    {
        test.run();
    }
    catch (std::exception e)
    {
        auto output = test.getOutput();
        std::cout << output << std::endl;
    }

    auto output = test.getOutput();
    std::cout << output << std::endl;
}

