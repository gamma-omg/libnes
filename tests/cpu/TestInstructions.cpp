#include <gtest/gtest.h>
#include "CPU.h"
#include "Memory.h"

using namespace nescore;

TEST(CPU, ADC_IMM)
{
    CPU cpu({ 0x69, 0x2 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, ADC_IMM_flag_V)
{
    CPU cpu({ 0x69, 0x7F,
              0x69, 0x1 });
    auto& registers = cpu.getRegisters();

    cpu.tick(2);

    ASSERT_EQ(registers.A, static_cast<uint8_t >(-128));
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::V));
}

TEST(CPU, ADC_IMM_flag_N_negative)
{
    CPU cpu({ 0x69, 0x2,
              0x69, 0x2 });
    auto& registers = cpu.getRegisters();

    cpu.tick(2);

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, ADC_IMM_flag_N_positive)
{
    CPU cpu({ 0x69, 0xF0,
              0x69, 0x2 });
    auto& registers = cpu.getRegisters();

    cpu.tick(2);

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, ADC_IMM_flag_Z_negative)
{
    CPU cpu({ 0x69, 0x1,
              0x69, 0x2 });
    auto& registers = cpu.getRegisters();

    cpu.tick(2);

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, ADC_IMM_flag_Z_positive)
{
    CPU cpu({ 0x69, 0x2,
              0x69, 0xFE });
    auto& registers = cpu.getRegisters();

    cpu.tick(2);

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
    ASSERT_EQ(registers.A, 0);
}

TEST(CPU, ADC_IMM_multiple)
{
    CPU cpu({ 0x69, 0x2,
              0x69, 0x6,
              0x69, 0x3});
    auto& registers = cpu.getRegisters();

    cpu.tick(3);

    ASSERT_EQ(registers.A, 11);
}

TEST(CPU, ADC_ZP)
{
    CPU cpu({ 0x69, 0x2,
              0x65, 0x5 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x5, 0x3);

    cpu.tick(2);

    ASSERT_EQ(registers.A, 0x5);
}

TEST(CPU, ADC_ZPX)
{
    CPU cpu({ 0x69, 0x2,
              0x75, 0x3 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x5, 0x3);
    registers.X = 0x2;

    cpu.tick(2);

    ASSERT_EQ(registers.A, 0x5);
}

TEST(CPU, ADC_ABS)
{
    CPU cpu({ 0x69, 0x2,
              0x6D, 0xFF, 0x10 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x10FF, 0x3);

    cpu.tick(2);

    ASSERT_EQ(registers.A, 0x5);
}

TEST(CPU, ADC_ABSX)
{
    CPU cpu({ 0x69, 0x2,
              0x7D, 0x05, 0x10 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x1008, 0x3);
    registers.X = 0x03;

    cpu.tick(2);

    ASSERT_EQ(registers.A, 0x5);
}

TEST(CPU, ADC_ABSY)
{
    CPU cpu({ 0x69, 0x2,
              0x79, 0x05, 0x10 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x1008, 0x3);
    registers.Y = 0x03;

    cpu.tick(2);

    ASSERT_EQ(registers.A, 0x5);
}

TEST(CPU, ADC_INDX)
{
    CPU cpu({ 0x69, 0x2,
              0x61, 0x05 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeShort(0x05, 0x1000);
    memory->writeByte(0x1002, 0x3);
    registers.X = 0x02;

    cpu.tick(2);

    ASSERT_EQ(registers.A, 0x5);
}

TEST(CPU, ADC_INDY)
{
    CPU cpu({ 0x69, 0x2,
              0x71, 0x05 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeShort(0x05, 0x1000);
    memory->writeByte(0x1002, 0x3);
    registers.Y = 0x02;

    cpu.tick(2);

    ASSERT_EQ(registers.A, 0x5);
}

TEST(CPU, AND_IMM)
{
    CPU cpu({ 0x29, 0b10110011 });

    auto& registers = cpu.getRegisters();
    registers.A = 0b11010111;

    cpu.tick();

    ASSERT_EQ(registers.A, 0b10010011);
}

TEST(CPU, AND_IMM_flag_Z_positive)
{
    CPU cpu({ 0x29, 0b11110000 });

    auto& registers = cpu.getRegisters();
    registers.A = 0b00001111;

    cpu.tick();

    ASSERT_EQ(registers.A, 0);
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, AND_IMM_flag_Z_negative)
{
    CPU cpu({ 0x29, 0b1 });

    auto& registers = cpu.getRegisters();
    registers.A = 0b1;

    cpu.tick();

    ASSERT_EQ(registers.A, 1);
    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, AND_IMM_flag_N_positive)
{
    CPU cpu({ 0x29, 0b11110000 });

    auto& registers = cpu.getRegisters();
    registers.A = 0b10001111;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, AND_IMM_flag_N_negative)
{
    CPU cpu({ 0x29, 0b01110001 });

    auto& registers = cpu.getRegisters();
    registers.A = 0b10001111;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, ASL_ACC)
{
    CPU cpu({ 0x0A });

    auto& registers = cpu.getRegisters();
    registers.A = 0b1;

    cpu.tick();

    ASSERT_EQ(registers.A, 0b10);
}

TEST(CPU, ASL_ACC_flag_Z_positive)
{
    CPU cpu({ 0x0A });

    auto& registers = cpu.getRegisters();
    registers.A = 0b0;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, ASL_ACC_flag_Z_negative)
{
    CPU cpu({ 0x0A });

    auto& registers = cpu.getRegisters();
    registers.A = 0b1;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, ASL_ACC_flag_N_positive)
{
    CPU cpu({ 0x0A });

    auto& registers = cpu.getRegisters();
    registers.A = 0b01000000;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, ASL_ACC_flag_N_negative)
{
    CPU cpu({ 0x0A });

    auto& registers = cpu.getRegisters();
    registers.A = 0b00100000;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, ASL_ACC_flag_C_positive)
{
    CPU cpu({ 0x0A });

    auto& registers = cpu.getRegisters();
    registers.A = 0b10000000;

    cpu.tick();

    ASSERT_EQ(registers.A, 0);
    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, ASL_ACC_flag_C_negative)
{
    CPU cpu({ 0x0A });

    auto& registers = cpu.getRegisters();
    registers.A = 0b01000000;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, BCC_jump)
{
    CPU cpu({ 0x90, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::C, false);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, BCC_dont_jump)
{
    CPU cpu({ 0x90, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::C, true);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 1);
}

TEST(CPU, BCS_jump)
{
    CPU cpu({ 0xB0, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::C, true);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, BCS_dont_jump)
{
    CPU cpu({ 0xB0, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::C, false);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 1);
}

TEST(CPU, BEQ_jump)
{
    CPU cpu({ 0xF0, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::Z, true);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, BEQ_dont_jump)
{
    CPU cpu({ 0xF0, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::Z, false);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 1);
}

TEST(CPU, BIT_ZP_flag_N_positive)
{
    CPU cpu({ 0x24, 0x01 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.A = 0b11001111;
    memory->writeByte(0x01, 0b11001010);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, BIT_ZP_flag_N_negative)
{
    CPU cpu({ 0x24, 0x01 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.A = 0b01001111;
    memory->writeByte(0x01, 0b11001010);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, BIT_ZP_flag_V_positive)
{
    CPU cpu({ 0x24, 0x01 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.A = 0b11001111;
    memory->writeByte(0x01, 0b11001010);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::V));
}

TEST(CPU, BIT_ZP_flag_V_negative)
{
    CPU cpu({ 0x24, 0x01 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.A = 0b10001111;
    memory->writeByte(0x01, 0b11001010);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::V));
}

TEST(CPU, BIT_ZP_flag_Z_positive)
{
    CPU cpu({ 0x24, 0x01 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.A = 0b11110000;
    memory->writeByte(0x01, 0b0001111);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, BIT_ZP_flag_Z_negative)
{
    CPU cpu({ 0x24, 0x01 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    registers.A = 0b11110001;
    memory->writeByte(0x01, 0b0001111);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, BMI_jump)
{
    CPU cpu({ 0x30, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::N, true);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, BMI_dont_jump)
{
    CPU cpu({ 0xF0, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::N, false);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 1);
}

TEST(CPU, BNE_jump)
{
    CPU cpu({ 0xD0, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::Z, false);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, BNE_dont_jump)
{
    CPU cpu({ 0xD0, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::Z, true);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 1);
}

TEST(CPU, BPL_jump)
{
    CPU cpu({ 0x10, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::N, false);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, BPL_dont_jump)
{
    CPU cpu({ 0x10, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::N, true);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 1);
}

TEST(CPU, BRK)
{
    CPU cpu({ 0x00, 0xFF, 0xFF, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeBrkVecor(Memory::ROM_OFFSET + 0x03);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, BVC_jump)
{
    CPU cpu({ 0x50, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::V, false);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, BVC_dont_jump)
{
    CPU cpu({ 0x50, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::V, true);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 1);
}

TEST(CPU, BVS_jump)
{
    CPU cpu({ 0x70, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::V, true);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 2);
}

TEST(CPU, BVS_dont_jump)
{
    CPU cpu({ 0x70, 0x05, 0x69, 0x01, 0x00, 0x69, 0x02 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::V, false);

    cpu.tick();
    cpu.tick();

    ASSERT_EQ(registers.A, 1);
}

TEST(CPU, CLC)
{
    CPU cpu({ 0x18 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::C, true);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, CLD)
{
    CPU cpu({ 0xD8 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::D, true);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::D));
}

TEST(CPU, CLI)
{
    CPU cpu({ 0x58 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::I, true);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::I));
}

TEST(CPU, CLV)
{
    CPU cpu({ 0xB8 });

    auto& registers = cpu.getRegisters();
    registers.setFlag(CPU::Registers::Flags::V, true);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::V));
}

TEST(CPU, CMP_flag_N_positive)
{
    CPU cpu({ 0xC9, 0x5 });

    auto& registers = cpu.getRegisters();
    registers.A = 0x2;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, CMP_flag_N_negative)
{
    CPU cpu({ 0xC9, 0x2 });

    auto& registers = cpu.getRegisters();
    registers.A = 0x5;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, CMP_flag_Z_positive)
{
    CPU cpu({ 0xC9, 0x2 });

    auto& registers = cpu.getRegisters();
    registers.A = 0x2;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, CMP_flag_Z_negative)
{
    CPU cpu({ 0xC9, 0x3 });

    auto& registers = cpu.getRegisters();
    registers.A = 0x2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, CMP_flag_C_negative)
{
    CPU cpu({ 0xC9, 0x3 });

    auto& registers = cpu.getRegisters();
    registers.A = 0x2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, CMP_flag_C_positive)
{
    CPU cpu({ 0xC9, 0x1 });

    auto& registers = cpu.getRegisters();
    registers.A = static_cast<uint8_t >(-128);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, CPX_flag_N_positive)
{
    CPU cpu({ 0xE0, 0x5 });

    auto& registers = cpu.getRegisters();
    registers.X = 0x2;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, CPX_flag_N_negative)
{
    CPU cpu({ 0xE0, 0x2 });

    auto& registers = cpu.getRegisters();
    registers.X = 0x5;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, CPX_flag_Z_positive)
{
    CPU cpu({ 0xE0, 0x2 });

    auto& registers = cpu.getRegisters();
    registers.X = 0x2;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, CPX_flag_Z_negative)
{
    CPU cpu({ 0xE0, 0x3 });

    auto& registers = cpu.getRegisters();
    registers.X = 0x2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, CPX_flag_C_negative)
{
    CPU cpu({ 0xE0, 0x3 });

    auto& registers = cpu.getRegisters();
    registers.X = 0x2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, CPX_flag_C_positive)
{
    CPU cpu({ 0xE0, 0x1 });

    auto& registers = cpu.getRegisters();
    registers.X = static_cast<uint8_t >(-128);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, CPY_flag_N_positive)
{
    CPU cpu({ 0xC0, 0x5 });

    auto& registers = cpu.getRegisters();
    registers.Y = 0x2;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, CPY_flag_N_negative)
{
    CPU cpu({ 0xC0, 0x2 });

    auto& registers = cpu.getRegisters();
    registers.Y = 0x5;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, CPY_flag_Z_positive)
{
    CPU cpu({ 0xC0, 0x2 });

    auto& registers = cpu.getRegisters();
    registers.Y = 0x2;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, CPY_flag_Z_negative)
{
    CPU cpu({ 0xC0, 0x3 });

    auto& registers = cpu.getRegisters();
    registers.Y = 0x2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, CPY_flag_C_negative)
{
    CPU cpu({ 0xC0, 0x3 });

    auto& registers = cpu.getRegisters();
    registers.Y = 0x2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, CPY_flag_C_positive)
{
    CPU cpu({ 0xC0, 0x1 });

    auto& registers = cpu.getRegisters();
    registers.Y = static_cast<uint8_t >(-128);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::C));
}

TEST(CPU, DEC)
{
    CPU cpu({ 0xC6, 0x03 });
    auto memory = cpu.getMemory();
    memory->writeByte(0x3, 5);

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x3), 4);
}

TEST(CPU, DEC_flag_N_positive)
{
    CPU cpu({ 0xC6, 0x03 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x3, 0);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, DEC_flag_N_negative)
{
    CPU cpu({ 0xC6, 0x03 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x3, 2);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, DEC_flag_Z_positive)
{
    CPU cpu({ 0xC6, 0x03 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x3, 1);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, DEC_flag_Z_negative)
{
    CPU cpu({ 0xC6, 0x03 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x3, 2);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, DEX)
{
    CPU cpu({ 0xCA });

    auto& registers = cpu.getRegisters();
    registers.X = 3;

    cpu.tick();

    ASSERT_EQ(registers.X, 2);
}

TEST(CPU, DEX_flag_N_positive)
{
    CPU cpu({ 0xCA });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, DEX_flag_N_negative)
{
    CPU cpu({ 0xCA });
    auto& registers = cpu.getRegisters();
    registers.X = 2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, DEX_flag_Z_positive)
{
    CPU cpu({ 0xCA });
    auto& registers = cpu.getRegisters();
    registers.X = 1;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, DEX_flag_Z_negative)
{
    CPU cpu({ 0xCA });
    auto& registers = cpu.getRegisters();
    registers.X = 2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, DEY)
{
    CPU cpu({ 0x88 });

    auto& registers = cpu.getRegisters();
    registers.Y = 3;

    cpu.tick();

    ASSERT_EQ(registers.Y, 2);
}

TEST(CPU, DEY_flag_N_positive)
{
    CPU cpu({ 0x88 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, DEY_flag_N_negative)
{
    CPU cpu({ 0x88 });
    auto& registers = cpu.getRegisters();
    registers.Y = 2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, DEY_flag_Z_positive)
{
    CPU cpu({ 0x88 });
    auto& registers = cpu.getRegisters();
    registers.Y = 1;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, DEY_flag_Z_negative)
{
    CPU cpu({ 0x88 });
    auto& registers = cpu.getRegisters();
    registers.Y = 2;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, EOR)
{
    CPU cpu({ 0x49, 0b11010011 });
    auto& registers = cpu.getRegisters();
    registers.A = 0b01100101;

    cpu.tick();

    ASSERT_EQ(registers.A, 0b10110110);
}

TEST(CPU, EOR_flag_N_positive)
{
    CPU cpu({ 0x49, 0b11010011 });
    auto& registers = cpu.getRegisters();
    registers.A = 0b01100101;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, EOR_flag_N_negative)
{
    CPU cpu({ 0x49, 0b11010011 });
    auto& registers = cpu.getRegisters();
    registers.A = 0b11100101;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, EOR_flag_Z_positive)
{
    CPU cpu({ 0x49, 0b11111111 });
    auto& registers = cpu.getRegisters();
    registers.A = 0b11111111;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, EOR_flag_Z_negative)
{
    CPU cpu({ 0x49, 0b11111110 });
    auto& registers = cpu.getRegisters();
    registers.A = 0b11111111;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, INC)
{
    CPU cpu({ 0xE6, 0x03 });
    auto memory = cpu.getMemory();
    memory->writeByte(0x03, 5);

    cpu.tick();

    ASSERT_EQ(memory->readByte(0x03), 6);
}

TEST(CPU, INC_flag_N_positive)
{
    CPU cpu({ 0xE6, 0x03 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x03, -5);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, INC_flag_N_negative)
{
    CPU cpu({ 0xE6, 0x03 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x03, 8);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, INC_flag_Z_positive)
{
    CPU cpu({ 0xE6, 0x03 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x03, -1);

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, INC_flag_Z_negative)
{
    CPU cpu({ 0xE6, 0x03 });

    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeByte(0x03, 8);

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, INX)
{
    CPU cpu({ 0xE8 });

    auto& registers = cpu.getRegisters();
    registers.X = 3;

    cpu.tick();

    ASSERT_EQ(registers.X, 4);
}

TEST(CPU, INX_flag_N_positive)
{
    CPU cpu({ 0xE8 });

    auto& registers = cpu.getRegisters();
    registers.X = -7;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, INX_flag_N_negative)
{
    CPU cpu({ 0xE8 });

    auto& registers = cpu.getRegisters();
    registers.X = 7;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, INX_flag_Z_positive)
{
    CPU cpu({ 0xE8 });

    auto& registers = cpu.getRegisters();
    registers.X = -1;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, INX_flag_Z_negative)
{
    CPU cpu({ 0xE8 });

    auto& registers = cpu.getRegisters();
    registers.X = 1;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, INY)
{
    CPU cpu({ 0xC8 });

    auto& registers = cpu.getRegisters();
    registers.Y = 3;

    cpu.tick();

    ASSERT_EQ(registers.Y, 4);
}

TEST(CPU, INY_flag_N_positive)
{
    CPU cpu({ 0xC8 });

    auto& registers = cpu.getRegisters();
    registers.Y = -7;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, INY_flag_N_negative)
{
    CPU cpu({ 0xC8 });

    auto& registers = cpu.getRegisters();
    registers.X = 7;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, INY_flag_Z_positive)
{
    CPU cpu({ 0xC8 });

    auto& registers = cpu.getRegisters();
    registers.Y = -1;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, INY_flag_Z_negative)
{
    CPU cpu({ 0xC8 });

    auto& registers = cpu.getRegisters();
    registers.X = 1;

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, JMP_ABS)
{
    CPU cpu({ 0x4C, 0x05, 0x00 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_EQ(registers.PC, 5);
}

TEST(CPU, JMP_IND)
{
    CPU cpu({ 0x6C, 0x04, 0x00 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();
    memory->writeShort(0x04, 0x07);

    cpu.tick();

    ASSERT_EQ(registers.PC, 7);
}

TEST(CPU, JSR)
{
    CPU cpu({ 0x20, 0x04, 0x00 });
    auto& registers = cpu.getRegisters();
    auto memory = cpu.getMemory();

    cpu.tick();

    ASSERT_EQ(registers.PC, 4);
    ASSERT_EQ(memory->readShortFromStack(registers.S + 2), Memory::ROM_OFFSET + 2);
}

TEST(CPU, LDA)
{
    CPU cpu({ 0xA9, 0x05 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_EQ(registers.A, 5);
}

TEST(CPU, LDA_flag_N_positive)
{
    CPU cpu({ 0xA9, static_cast<uint8_t>(-5) });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, LDA_flag_N_negative)
{
    CPU cpu({ 0xA9, 0x05 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, LDA_flag_Z_posititve)
{
    CPU cpu({ 0xA9, 0x00 });
    auto& registers = cpu.getRegisters();
    registers.A = 5;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, LDA_flag_Z_negative)
{
    CPU cpu({ 0xA9, 0x01 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, LDX)
{
    CPU cpu({ 0xA2, 0x05 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_EQ(registers.X, 5);
}

TEST(CPU, LDX_flag_N_positive)
{
    CPU cpu({ 0xA2, static_cast<uint8_t>(-5) });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, LDX_flag_N_negative)
{
    CPU cpu({ 0xA2, 0x05 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, LDX_flag_Z_posititve)
{
    CPU cpu({ 0xA2, 0x00 });
    auto& registers = cpu.getRegisters();
    registers.X = 5;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, LDX_flag_Z_negative)
{
    CPU cpu({ 0xA2, 0x01 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, LDY)
{
    CPU cpu({ 0xA0, 0x05 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_EQ(registers.Y, 5);
}

TEST(CPU, LDY_flag_N_positive)
{
    CPU cpu({ 0xA0, static_cast<uint8_t>(-5) });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, LDY_flag_N_negative)
{
    CPU cpu({ 0xA0, 0x05 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::N));
}

TEST(CPU, LDY_flag_Z_posititve)
{
    CPU cpu({ 0xA0, 0x00 });
    auto& registers = cpu.getRegisters();
    registers.Y = 5;

    cpu.tick();

    ASSERT_TRUE(registers.getFlag(CPU::Registers::Flags::Z));
}

TEST(CPU, LDY_flag_Z_negative)
{
    CPU cpu({ 0xA0, 0x01 });
    auto& registers = cpu.getRegisters();

    cpu.tick();

    ASSERT_FALSE(registers.getFlag(CPU::Registers::Flags::Z));
}