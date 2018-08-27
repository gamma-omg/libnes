#include <gtest/gtest.h>
#include <Memory.h>

using namespace nescore;

TEST(Memory, Read_byte)
{
    Memory memory({0x01, 0x02, 0x03});

    ASSERT_EQ(memory.readByte(0x0000), 0x01);
    ASSERT_EQ(memory.readByte(0x0001), 0x02);
    ASSERT_EQ(memory.readByte(0x0002), 0x03);
}

TEST(Memory, Read_short)
{
    Memory memory({0x01, 0x02, 0x03, 0x04});

    ASSERT_EQ(memory.readShort(0x0000), 0x0201);
    ASSERT_EQ(memory.readShort(0x0001), 0x0302);
    ASSERT_EQ(memory.readShort(0x0002), 0x0403);
}

TEST(Memory, Write_byte)
{
    Memory memory;

    memory.writeByte(0x05, 0xFE);

    ASSERT_EQ(memory.readByte(0x05), 0xFE);
}

TEST(Memory, Write_short)
{
    Memory memory;

    memory.writeShort(0x05, 0x0405);

    ASSERT_EQ(memory.readShort(0x05), 0x0405);
    ASSERT_EQ(memory.readByte(0x05), 0x05);
    ASSERT_EQ(memory.readByte(0x06), 0x04);
}

TEST(Memory, Read_RAM_Mirror)
{
    Memory memory({0x01, 0x02, 0x03});

    ASSERT_EQ(memory.readByte(0x0000), 0x01);
    ASSERT_EQ(memory.readByte(0x0800), 0x01);
    ASSERT_EQ(memory.readByte(0x1000), 0x01);
    ASSERT_EQ(memory.readByte(0x1800), 0x01);
}

TEST(Memory, Write_RAM_Mirror)
{
    Memory memory;

    memory.writeByte(0x05, 0xFE);

    ASSERT_EQ(memory.readByte(0x05), 0xFE);
    ASSERT_EQ(memory.readByte(0x0805), 0xFE);
    ASSERT_EQ(memory.readByte(0x1005), 0xFE);
    ASSERT_EQ(memory.readByte(0x1805), 0xFE);
}

TEST(Memory, Write_short_RAM_Mirror)
{
    Memory memory;

    memory.writeShort(0x05, 0xD0FE);

    ASSERT_EQ(memory.readShort(0x05), 0xD0FE);
    ASSERT_EQ(memory.readShort(0x0805), 0xD0FE);
    ASSERT_EQ(memory.readShort(0x1005), 0xD0FE);
    ASSERT_EQ(memory.readShort(0x1805), 0xD0FE);
}

TEST(Memory, Write_bytes)
{
    Memory memory;
    uint8_t buffer[] = { 0xF0, 0xFD, 0x78 };

    memory.writeBytes(Memory::ROM_OFFSET, reinterpret_cast<uint8_t*>(&buffer), 3);

    ASSERT_EQ(memory.readByte(Memory::ROM_OFFSET), 0xF0);
    ASSERT_EQ(memory.readByte(Memory::ROM_OFFSET + 1), 0xFD);
    ASSERT_EQ(memory.readByte(Memory::ROM_OFFSET + 2), 0x78);
}