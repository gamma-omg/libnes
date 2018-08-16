#include <gtest/gtest.h>
#include "Memory.h"

using namespace nescore;

TEST(Memory, Read_RAM)
{
    Memory memory({0x01, 0x02, 0x03});

    ASSERT_EQ(memory.readByte(0x0000), 0x01);
    ASSERT_EQ(memory.readByte(0x0001), 0x02);
    ASSERT_EQ(memory.readByte(0x0002), 0x03);
}

TEST(Memory, Read_RAM_Mirror)
{
    Memory memory({0x01, 0x02, 0x03});

    ASSERT_EQ(memory.readByte(0x0000), 0x01);
    ASSERT_EQ(memory.readByte(0x0800), 0x01);
    ASSERT_EQ(memory.readByte(0x1000), 0x01);
}