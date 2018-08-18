#include <gtest/gtest.h>
#include <fstream>
#include <rom/INESRom.h>

using namespace nescore;

#define READ_ROM(rom, file) \
    std::ifstream ifs; \
    ifs.open(file, std::ios::binary); \
    INESRom rom; \
    ifs >> rom; \

TEST(ROM, Load)
{
    READ_ROM(rom, "tests/rom/data/01-basics.nes");

    ASSERT_EQ(rom.getRpgRomBanks(), 2);
    ASSERT_EQ(rom.getChrRomBanks(), 1);
}

#undef READ_ROM