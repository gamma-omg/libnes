#include <cstdint>
#include <stdexcept>
#include <memory.h>
#include <functional>
#include "CPU.h"
#include "Memory.h"

#define READ_OPERAND(accessMethod) (this->*accessMethod)()
#define READ_ADDRESS(addressReader) (this->*addressReader)()

namespace nescore
{

CPU::Registers::Registers()
: A(0), S(0xFE), P(0), X(0), Y(0), PC(Memory::ROM_OFFSET)
{
}

void CPU::Registers::reset()
{
    A = P = X = Y = 0;
    S = 0xFE;
    PC = Memory::ROM_OFFSET;
}

void CPU::Registers::setFlag(nescore::CPU::Registers::Flags flag, bool value)
{
    auto bit = value ? 1 : 0;
    P ^= (-bit ^ P) & flag;
}

bool CPU::Registers::getFlag(nescore::CPU::Registers::Flags flag) const
{
    return P & flag;
}


CPU::CPU(std::shared_ptr<Memory> memory)
    : _memory(memory)
{
   _registers.reset();
   initHandlers();
}

CPU::CPU(const std::vector<uint8_t>& operations)
    : CPU(std::make_shared<Memory>())
{
    _memory->loadROM(operations);
}

void CPU::tick()
{
    auto opcode = _memory->readByte(_registers.PC++);
    auto handler = _instructions[opcode];
    if (!handler)
    {
        throw std::runtime_error("Invalid opcode: " + opcode);
    }

    handler();
}

void CPU::tick(int count)
{
    for (int i = 0; i < count; ++i)
    {
        tick();
    }
}

const CPU::Registers& CPU::getRegisters() const
{
    return _registers;
}

CPU::Registers &CPU::getRegisters()
{
    return _registers;
}

std::shared_ptr<Memory> CPU::getMemory()
{
    return _memory;
}

void CPU::initHandlers()
{
    memset(&_instructions, 0, 0xFF);

    _instructions[0x69] = [this]() { op_adc(access_imm); };
    _instructions[0x65] = [this]() { op_adc(access_zp); };
    _instructions[0x75] = [this]() { op_adc(access_zpx); };
    _instructions[0x6D] = [this]() { op_adc(access_abs); };
    _instructions[0x7D] = [this]() { op_adc(access_absx); };
    _instructions[0x79] = [this]() { op_adc(access_absy); };
    _instructions[0x61] = [this]() { op_adc(access_indx); };
    _instructions[0x71] = [this]() { op_adc(access_indy); };

    _instructions[0x29] = [this]() { op_and(access_imm); };
    _instructions[0x25] = [this]() { op_and(access_zp); };
    _instructions[0x35] = [this]() { op_and(access_zpx); };
    _instructions[0x2D] = [this]() { op_and(access_abs); };
    _instructions[0x3D] = [this]() { op_and(access_absx); };
    _instructions[0x39] = [this]() { op_and(access_absy); };
    _instructions[0x21] = [this]() { op_and(access_indx); };
    _instructions[0x31] = [this]() { op_and(access_indy); };

    _instructions[0x0A] = [this]() { op_asl(access_acc); };
    _instructions[0x06] = [this]() { op_asl(access_zp); };
    _instructions[0x16] = [this]() { op_asl(access_zpx); };
    _instructions[0x0E] = [this]() { op_asl(access_abs); };
    _instructions[0x1E] = [this]() { op_asl(access_absx); };

    _instructions[0x90] = [this]() { op_bcc(access_rel); };
    _instructions[0xB0] = [this]() { op_bcs(access_rel); };
    _instructions[0xF0] = [this]() { op_beq(access_rel); };
    _instructions[0x30] = [this]() { op_bmi(access_rel); };
    _instructions[0xD0] = [this]() { op_bne(access_rel); };
    _instructions[0x10] = [this]() { op_bpl(access_rel); };
    _instructions[0x50] = [this]() { op_bvc(access_rel); };
    _instructions[0x70] = [this]() { op_bvs(access_rel); };

    _instructions[0x24] = [this]() { op_bit(access_zp); };
    _instructions[0x2C] = [this]() { op_bit(access_abs); };

    _instructions[0x00] = [this]() { op_brk(access_impl); };

    _instructions[0x18] = [this]() { op_clc(access_impl); };
    _instructions[0xD8] = [this]() { op_cld(access_impl); };
    _instructions[0x58] = [this]() { op_cli(access_impl); };
    _instructions[0xB8] = [this]() { op_clv(access_impl); };

    _instructions[0xC9] = [this]() { op_cmp(access_imm); };
    _instructions[0xC5] = [this]() { op_cmp(access_zp); };
    _instructions[0xD5] = [this]() { op_cmp(access_zpx); };
    _instructions[0xCD] = [this]() { op_cmp(access_abs); };
    _instructions[0xDD] = [this]() { op_cmp(access_absx); };
    _instructions[0xD9] = [this]() { op_cmp(access_absy); };
    _instructions[0xC1] = [this]() { op_cmp(access_indx); };
    _instructions[0xD1] = [this]() { op_cmp(access_indy); };

    _instructions[0xE0] = [this]() { op_cpx(access_imm); };
    _instructions[0xE4] = [this]() { op_cpx(access_zp); };
    _instructions[0xEC] = [this]() { op_cpx(access_abs); };
    _instructions[0xC0] = [this]() { op_cpy(access_imm); };
    _instructions[0xC4] = [this]() { op_cpy(access_zp); };
    _instructions[0xCC] = [this]() { op_cpy(access_abs); };

    _instructions[0xC6] = [this]() { op_dec(address_zp); };
    _instructions[0xD6] = [this]() { op_dec(address_zpx); };
    _instructions[0xCE] = [this]() { op_dec(address_abs); };
    _instructions[0xDE] = [this]() { op_dec(address_absx); };
    _instructions[0xCA] = [this]() { op_dex(access_impl); };
    _instructions[0x88] = [this]() { op_dey(access_impl); };

    _instructions[0x49] = [this]() { op_eor(access_imm); };
    _instructions[0x45] = [this]() { op_eor(access_zp); };
    _instructions[0x55] = [this]() { op_eor(access_zpx); };
    _instructions[0x4D] = [this]() { op_eor(access_abs); };
    _instructions[0x5D] = [this]() { op_eor(access_absx); };
    _instructions[0x59] = [this]() { op_eor(access_absy); };
    _instructions[0x41] = [this]() { op_eor(access_indx); };
    _instructions[0x51] = [this]() { op_eor(access_indy); };

    _instructions[0xE6] = [this]() { op_inc(address_zp); };
    _instructions[0xF6] = [this]() { op_inc(address_zpx); };
    _instructions[0xEE] = [this]() { op_inc(address_abs); };
    _instructions[0xFE] = [this]() { op_inc(address_absx); };
    _instructions[0xE8] = [this]() { op_inx(access_impl); };
    _instructions[0xC8] = [this]() { op_iny(access_impl); };

    _instructions[0x4C] = [this]() { op_jmp(address_abs); };
    _instructions[0x6C] = [this]() { op_jmp(address_ind); };
    _instructions[0x20] = [this]() { op_jsr(address_abs); };

    _instructions[0xA9] = [this]() { op_lda(access_imm); };
    _instructions[0xA5] = [this]() { op_lda(access_zp); };
    _instructions[0xB5] = [this]() { op_lda(access_zpx); };
    _instructions[0xAD] = [this]() { op_lda(access_abs); };
    _instructions[0xBD] = [this]() { op_lda(access_absx); };
    _instructions[0xB9] = [this]() { op_lda(access_absy); };
    _instructions[0xA1] = [this]() { op_lda(access_indx); };
    _instructions[0xB1] = [this]() { op_lda(access_indy); };
    _instructions[0xA2] = [this]() { op_ldx(access_imm); };
    _instructions[0xA6] = [this]() { op_ldx(access_zp); };
    _instructions[0xB6] = [this]() { op_ldx(access_zpy); };
    _instructions[0xAE] = [this]() { op_ldx(access_abs); };
    _instructions[0xBE] = [this]() { op_ldx(access_absy); };
    _instructions[0xA0] = [this]() { op_ldy(access_imm); };
    _instructions[0xA4] = [this]() { op_ldy(access_zp); };
    _instructions[0xB4] = [this]() { op_ldy(access_zpx); };
    _instructions[0xAC] = [this]() { op_ldy(access_abs); };
    _instructions[0xBC] = [this]() { op_ldy(access_absx); };
}

void CPU::op_adc(CPU::AccessMethod accessMethod)
{
    int8_t operand = READ_OPERAND(accessMethod);
    int16_t result = _registers.A + operand;

    _registers.setFlag(Registers::Flags::C, result > 0xFF);
    _registers.setFlag(Registers::Flags::V, result > 0x7F);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::N, result & 0b10000000);
    _registers.A = static_cast<int8_t>(result);
}

void CPU::op_and(CPU::AccessMethod accessMethod)
{
    int8_t operand = READ_OPERAND(accessMethod);
    uint8_t result = operand & _registers.A;

    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::N, result & 0b10000000);
    _registers.A = result;
}

void CPU::op_asl(CPU::AccessMethod accessMethod)
{
    int8_t operand = READ_OPERAND(accessMethod);
    int8_t result = operand << 1;

    _registers.setFlag(Registers::Flags::C, operand & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::N, result & 0b10000000);
    _registers.A = result;
}

void CPU::op_bcc(CPU::AccessMethod accessMethod)
{
    conditionalJump(Registers::Flags::C, false, accessMethod);
}

void CPU::op_bcs(CPU::AccessMethod accessMethod)
{
    conditionalJump(Registers::Flags::C, true, accessMethod);
}

void CPU::op_beq(CPU::AccessMethod accessMethod)
{
    conditionalJump(Registers::Flags::Z, true, accessMethod);
}

void CPU::op_bit(CPU::AccessMethod accessMethod)
{
    uint8_t operand = READ_OPERAND(accessMethod);
    uint8_t result = _registers.A & operand;

    _registers.setFlag(Registers::Flags::N, result & 0b10000000);
    _registers.setFlag(Registers::Flags::V, result & 0b01000000);
    _registers.setFlag(Registers::Flags::Z, result == 0);
}

void CPU::op_bmi(CPU::AccessMethod accessMethod)
{
    conditionalJump(Registers::Flags::N, true, accessMethod);
}

void CPU::op_bne(CPU::AccessMethod accessMethod)
{
    conditionalJump(Registers::Flags::Z, false, accessMethod);
}

void CPU::op_bpl(CPU::AccessMethod accessMethod)
{
    conditionalJump(Registers::Flags::N, false, accessMethod);
}

void CPU::op_brk(CPU::AccessMethod)
{
    _registers.PC = _memory->readBrkVector();
    _registers.setFlag(Registers::Flags::I, true);
}

void CPU::op_bvc(CPU::AccessMethod)
{
    conditionalJump(Registers::Flags::V, false, access_rel);
}

void CPU::op_bvs(CPU::AccessMethod)
{
    conditionalJump(Registers::Flags::V, true, access_rel);
}

void CPU::op_clc(CPU::AccessMethod)
{
    _registers.setFlag(Registers::Flags::C, false);
}

void CPU::op_cld(CPU::AccessMethod)
{
    _registers.setFlag(Registers::Flags::D, false);
}

void CPU::op_cli(CPU::AccessMethod)
{
    _registers.setFlag(Registers::Flags::I, false);
}

void CPU::op_clv(CPU::AccessMethod)
{
    _registers.setFlag(Registers::Flags::V, false);
}

void CPU::op_cmp(CPU::AccessMethod accessMethod)
{
    uint8_t operand = READ_OPERAND(accessMethod);
    uint8_t result = _registers.A - operand;

    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::C, _registers.A >= operand );
}

void CPU::op_cpx(CPU::AccessMethod accessMethod)
{
    uint8_t operand = READ_OPERAND(accessMethod);
    uint8_t result = _registers.X - operand;

    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::C, _registers.X >= operand);
}

void CPU::op_cpy(CPU::AccessMethod accessMethod)
{
    uint8_t operand = READ_OPERAND(accessMethod);
    uint8_t result = _registers.Y - operand;

    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::C, _registers.Y >= operand);
}

void CPU::op_dec(CPU::AddressReader addressReader)
{
    uint16_t address = READ_ADDRESS(addressReader);
    int8_t operand = _memory->readByte(address);

    operand--;
    _registers.setFlag(Registers::Flags::N, operand & 0x80);
    _registers.setFlag(Registers::Flags::Z, operand == 0);
    _memory->writeByte(address, operand);
}

void CPU::op_dex(CPU::AccessMethod)
{
    _registers.X--;
    _registers.setFlag(Registers::Flags::N, _registers.X & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.X == 0);
}

void CPU::op_dey(CPU::AccessMethod)
{
    _registers.Y--;
    _registers.setFlag(Registers::Flags::N, _registers.Y & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.Y == 0);
}

void CPU::op_eor(CPU::AccessMethod accessMethod)
{
    int8_t operand = READ_OPERAND(accessMethod);
    int8_t result = _registers.A ^ operand;

    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.A = result;
}

void CPU::op_inc(CPU::AddressReader addressReader)
{
    uint16_t address = READ_ADDRESS(addressReader);
    int8_t operand = _memory->readByte(address);

    operand++;
    _registers.setFlag(Registers::Flags::N, operand & 0x80);
    _registers.setFlag(Registers::Flags::Z, operand == 0);
    _memory->writeByte(address, operand);
}

void CPU::op_inx(CPU::AccessMethod)
{
    _registers.X++;
    _registers.setFlag(Registers::Flags::N, _registers.X & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.X == 0);
}

void CPU::op_iny(CPU::AccessMethod)
{
    _registers.Y++;
    _registers.setFlag(Registers::Flags::N, _registers.Y & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.Y == 0);
}

void CPU::op_jmp(CPU::AddressReader addressReader)
{
    _registers.PC = READ_ADDRESS(addressReader);
}

void CPU::op_jsr(CPU::AddressReader addressReader)
{
    _memory->writeShortToStack(_registers.S, _registers.PC + 2);
    _registers.S -= 2;
    _registers.PC = READ_ADDRESS(addressReader);
}

void CPU::op_lda(CPU::AccessMethod accessMethod)
{
    _registers.A = READ_OPERAND(accessMethod);
    _registers.setFlag(Registers::Flags::N, _registers.A & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.A == 0);
}

void CPU::op_ldx(CPU::AccessMethod accessMethod)
{
    _registers.X = READ_OPERAND(accessMethod);
    _registers.setFlag(Registers::Flags::N, _registers.X & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.X == 0);
}

void CPU::op_ldy(CPU::AccessMethod accessMethod)
{
    _registers.Y = READ_OPERAND(accessMethod);
    _registers.setFlag(Registers::Flags::N, _registers.Y & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.Y == 0);
}

void CPU::conditionalJump(CPU::Registers::Flags flag, bool state, CPU::AccessMethod accessMethod)
{
    if (_registers.getFlag(flag) == state)
    {
        auto offset = static_cast<int8_t>(READ_OPERAND(accessMethod));
        _registers.PC += offset - 1;
    }
    else
    {
        _registers.PC++;
    }
}

uint8_t CPU::access_acc()
{
    return _registers.A;
}

uint8_t CPU::access_imm()
{
    return _memory->readByte(_registers.PC++);
}

uint8_t CPU::access_zp()
{
    uint8_t address = _memory->readByte(_registers.PC++);
    return _memory->readByte(address);
}

uint8_t CPU::access_zpx()
{
    uint8_t address = _memory->readByte(_registers.PC++);
    return _memory->readByte(address + _registers.X);
}

uint8_t CPU::access_zpy()
{
    uint8_t address = _memory->readByte(_registers.PC++);
    return _memory->readByte(address + _registers.Y);
}

uint8_t CPU::access_abs()
{
    uint16_t address = _memory->readShort(_registers.PC);
    _registers.PC += 2;

    return _memory->readByte(address);
}

uint8_t CPU::access_absx()
{
    uint16_t address = _memory->readShort(_registers.PC);
    _registers.PC += 2;

    return _memory->readByte(address + _registers.X);
}

uint8_t CPU::access_absy()
{
    uint16_t address = _memory->readShort(_registers.PC);
    _registers.PC += 2;

    return _memory->readByte(address + _registers.Y);
}

uint8_t CPU::access_indx()
{
    uint8_t index = _memory->readByte(_registers.PC++) + _registers.X;
    uint16_t address = _memory->readShort(index);
    return _memory->readByte(address);
}

uint8_t CPU::access_indy()
{
    uint8_t index = _memory->readByte(_registers.PC++) + _registers.Y;
    uint16_t address = _memory->readShort(index);
    return _memory->readByte(address);
}

uint8_t CPU::access_rel()
{
    return _memory->readByte(_registers.PC);
}

uint8_t CPU::access_impl()
{
    return 0;
}

uint16_t CPU::address_zp()
{
    return _memory->readByte(_registers.PC++);
}

uint16_t CPU::address_zpx()
{
    return _memory->readByte(_registers.PC++) + _registers.X;
}

uint16_t CPU::address_abs()
{
    uint16_t address = _memory->readShort(_registers.PC);
    _registers.PC += 2;

    return address;
}

uint16_t CPU::address_absx()
{
    uint16_t address = _memory->readShort(_registers.PC++) + _registers.X;
    _registers.PC += 2;

    return address;
}

uint16_t CPU::address_ind()
{
    uint16_t address = _memory->readShort(_registers.PC);
    _registers.PC += 2;

    return _memory->readShort(address);
}

#undef READ_ADDRESS
#undef READ_OPERAND

}