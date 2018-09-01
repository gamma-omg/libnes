#include <gtest/gtest.h>
#include <fstream>
#include <rom/INESRom.h>
#include <mappers/MapperFactory.h>
#include <cpu/CPU.h>
#include <Memory.h>
#include "utils/TestProgram.h"

using namespace nescore;

TEST(CPU, Implied)
{
    TestProgram test("tests/data/01-implied.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, Immediate)
{
    TestProgram test("tests/data/02-immediate.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, ZeroPage)
{
    TestProgram test("tests/data/03-zero_page.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, ZeroPage_XY)
{
    TestProgram test("tests/data/04-zp_xy.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, Absolute)
{
    TestProgram test("tests/data/05-absolute.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, Absolute_XY)
{
    TestProgram test("tests/data/06-abs_xy.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, Indirect_X)
{
    TestProgram test("tests/data/07-ind_x.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, Indirect_Y)
{
    TestProgram test("tests/data/08-ind_y.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, Branches)
{
    TestProgram test("tests/data/09-branches.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, Stack)
{
    TestProgram test("tests/data/10-stack.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}

TEST(CPU, Special)
{
    TestProgram test("tests/data/11-special.nes");

    int status = test.run();

    auto& out = status == 0 ? std::cout : std::cerr;
    out << std::endl << test.getOutput() << std::endl;
    out.flush();
    ASSERT_EQ(status, 0);
}
