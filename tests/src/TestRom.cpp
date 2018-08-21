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
    READ_ROM(rom, "tests/data/01-basics.nes");

    ASSERT_EQ(rom.getPrgRomBanks(), 2);
    ASSERT_EQ(rom.getChrRomBanks(), 1);
    ASSERT_EQ(rom.getMapper(), 0);
    ASSERT_FALSE(rom.isNES2Format());
}

#undef READ_ROM