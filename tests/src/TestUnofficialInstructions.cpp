#include <gtest/gtest.h>
#include <CPU.h>
#include <Memory.h>

using namespace nescore;

TEST(CPU, ALR)
{
    CPU cpu({ 0x4B, 0b11011001 });
    auto& registers = cpu.getRegisters();
    registers.A = 0b01110101;

    cpu.tick();

    ASSERT_EQ(registers.A, 0b00101000);
    ASSERT_EQ(cpu.getCycle(), 2);
}

TEST(CPU, ANC_1)
{
    CPU cpu({ 0x0B, 0b11011001 });
    auto& registers = cpu.getRegisters();
    registers.A = 0b11110101;

    cpu.tick();

    ASSERT_EQ(registers.A, 0b11010001);
    ASSERT_EQ(cpu.getCycle(), 2);
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, ANC_2)
{
    CPU cpu({ 0x2B, 0b11011001 });
    auto& registers = cpu.getRegisters();
    registers.A = 0b11110101;

    cpu.tick();

    ASSERT_EQ(registers.A, 0b11010001);
    ASSERT_EQ(cpu.getCycle(), 2);
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::C));
}


TEST(CPU, ARR)
{
    CPU cpu({ 0x6B, 0b11011001 });
    auto& registers = cpu.getRegisters();
    registers.A = 0b11110101;
    registers.setFlag(CPU::Registers::Flags::C, true);

    cpu.tick();

    ASSERT_EQ(registers.A, 0b11101000);
    ASSERT_EQ(cpu.getCycle(), 2);
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::C));
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::V));
}

TEST(CPU, AXS)
{
    CPU cpu({ 0xCB, static_cast<uint8_t>(-5) });
    auto& registers = cpu.getRegisters();
    registers.A = 0b11110101;
    registers.X = 0b10100110;

    cpu.tick();

    ASSERT_EQ(registers.X, 0b10100100 + 5);
    ASSERT_EQ(cpu.getCycle(), 2);
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::C));
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, LAX)
{
    CPU cpu({ 0xA7, 0x05 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x05, 0xFF);

    cpu.tick();

    ASSERT_EQ(registers.X, 0xFF);
    ASSERT_EQ(registers.A, 0xFF);
    ASSERT_EQ(cpu.getCycle(), 3);
}

TEST(CPU, SAX)
{
    CPU cpu({ 0x87, 0x05 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.A = 0b01011010;
    registers.X = 0b11010011;

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x05), 0b01010010);
    ASSERT_EQ(cpu.getCycle(), 3);
}

TEST(CPU, DCP)
{
    CPU cpu({ 0xC7, 0x05 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x05, 20);
    registers.A = 10;

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x05), 19);
    ASSERT_EQ(cpu.getCycle(), 5);
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, ISC)
{
    CPU cpu({ 0xE7, 0x05 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x05, 5);
    registers.A = 10;
    registers.setFlag(CPU::Registers::Flags::C, true);

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x05), 6);
    ASSERT_EQ(registers.A, 4);
    ASSERT_EQ(cpu.getCycle(), 5);
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, RLA)
{
    CPU cpu({ 0x27, 0x05 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x05, 0b11010011);
    registers.A = 0b01001100;

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x05), 0b10100110);
    ASSERT_EQ(registers.A, 0b00000100);
    ASSERT_EQ(cpu.getCycle(), 5);
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, RRA)
{
    CPU cpu({ 0x67, 0x05 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x05, 0b11010010);
    registers.A = 7;

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x05), 0b01101001);
    ASSERT_EQ(registers.A, 0b01101001 + 7);
    ASSERT_EQ(cpu.getCycle(), 5);
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, SLO)
{
    CPU cpu({ 0x07, 0x05 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x05, 0b11010010);
    registers.A = 0b10011011;

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x05), 0b10100100);
    ASSERT_EQ(registers.A, 0b10111111);
    ASSERT_EQ(cpu.getCycle(), 5);
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, SRE)
{
    CPU cpu({ 0x47, 0x05 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x05, 0b11010010);
    registers.A = 0b10011011;

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x05), 0b01101001);
    ASSERT_EQ(registers.A, 0b11110010);
    ASSERT_EQ(cpu.getCycle(), 5);
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, KIL)
{
    CPU cpu({ 0x02,
              0x69, 0x02 });
    auto& registers = cpu.getRegisters();

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.PC, Memory::ROM_OFFSET + 1);
    ASSERT_EQ(registers.A, 0);
}

TEST(CPU, LAS)
{
    CPU cpu({ 0xBB, 0x05 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.Y = 3;
    registers.S = 0b10001011;
    memory->writeByte(0x08, 0b11001101);

    cpu.tick();

    ASSERT_EQ(registers.A, 0b10001001);
    ASSERT_EQ(registers.S, 0b10001001);
    ASSERT_EQ(registers.X, 0b10001001);
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, SXA)
{
    CPU cpu({ 0x9E, 0x05, 0x10 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.X = 3;
    registers.Y = 3;

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x1108), 1);
    ASSERT_EQ(cpu.getCycle(), 5);
}

TEST(CPU, SYA)
{
    CPU cpu({ 0x9C, 0x05, 0x10 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.X = 3;
    registers.Y = 3;

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x1108), 1);
    ASSERT_EQ(cpu.getCycle(), 5);
}

TEST(CPU, XAS)
{
    CPU cpu({ 0x9B, 0x05, 0x10 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.A = 0b11001101;
    registers.X = 3;
    registers.Y = 3;

    cpu.tick();

    ASSERT_EQ(registers.S, 1);
    ASSERT_EQ(memory->readByte(0x1008), 1);
    ASSERT_EQ(cpu.getCycle(), 5);
}