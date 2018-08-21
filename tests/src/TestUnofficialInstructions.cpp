#include <gtest/gtest.h>
#include <CPU.h>

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