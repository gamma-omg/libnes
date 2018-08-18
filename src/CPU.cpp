#include <cstdint>
#include <stdexcept>
#include <memory.h>
#include <functional>
#include "CPU.h"
#include "Memory.h"

#include "access/ABS.h"
#include "access/ABSX.h"
#include "access/ABSY.h"
#include "access/ACC.h"
#include "access/IMM.h"
#include "access/IMPL.h"
#include "access/INDX.h"
#include "access/INDY.h"
#include "access/ZP.h"
#include "access/ZPX.h"
#include "access/ZPY.h"

namespace nescore
{

CPU::Registers::Registers()
: A(0), S(0xFE), P(0), X(0), Y(0), PC(Memory::ROM_OFFSET)
{
}

void CPU::Registers::reset()
{
    A = P = X = Y = 0;
    S = 0xFD;
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
    , _cycle(0)
{
   _registers.reset();
    setupInstructions();
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

    _cycle++;
    _cycle += (this->*handler)();
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

cpu_cycle_t CPU::getCycle() const
{
    return _cycle;
}

void CPU::setupInstructions()
{
    memset(&_instructions, 0, sizeof(InstructionHandler) * 0xFF);

    _instructions[0x69] = CPU::op_adc<IMM>;
    _instructions[0x65] = CPU::op_adc<ZP>;
    _instructions[0x75] = CPU::op_adc<ZPX>;
    _instructions[0x6D] = CPU::op_adc<ABS>;
    _instructions[0x7D] = CPU::op_adc<ABSX>;
    _instructions[0x79] = CPU::op_adc<ABSY>;
    _instructions[0x61] = CPU::op_adc<INDX>;
    _instructions[0x71] = CPU::op_adc<INDY>;

    _instructions[0x29] = CPU::op_and<IMM>;
    _instructions[0x25] = CPU::op_and<ZP>;
    _instructions[0x35] = CPU::op_and<ZPX>;
    _instructions[0x2D] = CPU::op_and<ABS>;
    _instructions[0x3D] = CPU::op_and<ABSX>;
    _instructions[0x39] = CPU::op_and<ABSY>;
    _instructions[0x21] = CPU::op_and<INDX>;
    _instructions[0x31] = CPU::op_and<INDY>;

    _instructions[0x0A] = CPU::op_asl<ACC>;
    _instructions[0x06] = CPU::op_asl<ZP>;
    _instructions[0x16] = CPU::op_asl<ZPX>;
    _instructions[0x0E] = CPU::op_asl<ABS>;
    _instructions[0x1E] = CPU::op_asl<ABSX>;

    _instructions[0x90] = CPU::op_bcc;
    _instructions[0xB0] = CPU::op_bcs;
    _instructions[0xF0] = CPU::op_beq;
    _instructions[0x30] = CPU::op_bmi;
    _instructions[0xD0] = CPU::op_bne;
    _instructions[0x10] = CPU::op_bpl;
    _instructions[0x50] = CPU::op_bvc;
    _instructions[0x70] = CPU::op_bvs;

    _instructions[0x24] = CPU::op_bit<ZP>;
    _instructions[0x2C] = CPU::op_bit<ABS>;

    _instructions[0x00] = CPU::op_brk;

    _instructions[0x18] = CPU::op_clc;
    _instructions[0xD8] = CPU::op_cld;
    _instructions[0x58] = CPU::op_cli;
    _instructions[0xB8] = CPU::op_clv;

    _instructions[0xC9] = CPU::op_cmp<IMM>;
    _instructions[0xC5] = CPU::op_cmp<ZP>;
    _instructions[0xD5] = CPU::op_cmp<ZPX>;
    _instructions[0xCD] = CPU::op_cmp<ABS>;
    _instructions[0xDD] = CPU::op_cmp<ABSX>;
    _instructions[0xD9] = CPU::op_cmp<ABSY>;
    _instructions[0xC1] = CPU::op_cmp<INDX>;
    _instructions[0xD1] = CPU::op_cmp<INDY>;

    _instructions[0xE0] = CPU::op_cpx<IMM>;
    _instructions[0xE4] = CPU::op_cpx<ZP>;
    _instructions[0xEC] = CPU::op_cpx<ABS>;
    _instructions[0xC0] = CPU::op_cpy<IMM>;
    _instructions[0xC4] = CPU::op_cpy<ZP>;
    _instructions[0xCC] = CPU::op_cpy<ABS>;

    _instructions[0xC6] = CPU::op_dec<ZP>;
    _instructions[0xD6] = CPU::op_dec<ZPX>;
    _instructions[0xCE] = CPU::op_dec<ABS>;
    _instructions[0xDE] = CPU::op_dec<ABSX>;
    _instructions[0xCA] = CPU::op_dex;
    _instructions[0x88] = CPU::op_dey;

    _instructions[0x49] = CPU::op_eor<IMM>;
    _instructions[0x45] = CPU::op_eor<ZP>;
    _instructions[0x55] = CPU::op_eor<ZPX>;
    _instructions[0x4D] = CPU::op_eor<ABS>;
    _instructions[0x5D] = CPU::op_eor<ABSX>;
    _instructions[0x59] = CPU::op_eor<ABSY>;
    _instructions[0x41] = CPU::op_eor<INDX>;
    _instructions[0x51] = CPU::op_eor<INDY>;

    _instructions[0xE6] = CPU::op_inc<ZP>;
    _instructions[0xF6] = CPU::op_inc<ZPX>;
    _instructions[0xEE] = CPU::op_inc<ABS>;
    _instructions[0xFE] = CPU::op_inc<ABSX>;
    _instructions[0xE8] = CPU::op_inx;
    _instructions[0xC8] = CPU::op_iny;

    _instructions[0x4C] = CPU::op_jmp_abs;
    _instructions[0x6C] = CPU::op_jmp_ind;
    _instructions[0x20] = CPU::op_jsr;

    _instructions[0xA9] = CPU::op_lda<IMM>;
    _instructions[0xA5] = CPU::op_lda<ZP>;
    _instructions[0xB5] = CPU::op_lda<ZPX>;
    _instructions[0xAD] = CPU::op_lda<ABS>;
    _instructions[0xBD] = CPU::op_lda<ABSX>;
    _instructions[0xB9] = CPU::op_lda<ABSY>;
    _instructions[0xA1] = CPU::op_lda<INDX>;
    _instructions[0xB1] = CPU::op_lda<INDY>;
    _instructions[0xA2] = CPU::op_ldx<IMM>;
    _instructions[0xA6] = CPU::op_ldx<ZP>;
    _instructions[0xB6] = CPU::op_ldx<ZPY>;
    _instructions[0xAE] = CPU::op_ldx<ABS>;
    _instructions[0xBE] = CPU::op_ldx<ABSY>;
    _instructions[0xA0] = CPU::op_ldy<IMM>;
    _instructions[0xA4] = CPU::op_ldy<ZP>;
    _instructions[0xB4] = CPU::op_ldy<ZPX>;
    _instructions[0xAC] = CPU::op_ldy<ABS>;
    _instructions[0xBC] = CPU::op_ldy<ABSX>;

    _instructions[0x4A] = CPU::op_lsr<ACC>;
    _instructions[0x46] = CPU::op_lsr<ZP>;
    _instructions[0x56] = CPU::op_lsr<ZPX>;
    _instructions[0x4E] = CPU::op_lsr<ABS>;
    _instructions[0x5E] = CPU::op_lsr<ABSX>;

    _instructions[0xEA] = CPU::op_nop;

    _instructions[0x09] = CPU::op_ora<IMM>;
    _instructions[0x05] = CPU::op_ora<ZP>;
    _instructions[0x15] = CPU::op_ora<ZPX>;
    _instructions[0x0D] = CPU::op_ora<ABS>;
    _instructions[0x1D] = CPU::op_ora<ABSX>;
    _instructions[0x19] = CPU::op_ora<ABSY>;
    _instructions[0x01] = CPU::op_ora<INDX>;
    _instructions[0x11] = CPU::op_ora<INDY>;

    _instructions[0x48] = CPU::op_pha;
    _instructions[0x08] = CPU::op_php;
    _instructions[0x68] = CPU::op_pla;
    _instructions[0x28] = CPU::op_plp;

    _instructions[0x2A] = CPU::op_rol<ACC>;
    _instructions[0x26] = CPU::op_rol<ZP>;
    _instructions[0x36] = CPU::op_rol<ZPX>;
    _instructions[0x2E] = CPU::op_rol<ABS>;
    _instructions[0x3E] = CPU::op_rol<ABSX>;
    _instructions[0x6A] = CPU::op_ror<ACC>;
    _instructions[0x66] = CPU::op_ror<ZP>;
    _instructions[0x76] = CPU::op_ror<ZPX>;
    _instructions[0x6E] = CPU::op_ror<ABS>;
    _instructions[0x7E] = CPU::op_ror<ABSX>;

    _instructions[0x4D] = CPU::op_rti;
    _instructions[0x60] = CPU::op_rts;

    _instructions[0xE9] = CPU::op_sbc<IMM>;
    _instructions[0xE5] = CPU::op_sbc<ZP>;
    _instructions[0xF5] = CPU::op_sbc<ZPX>;
    _instructions[0xED] = CPU::op_sbc<ABS>;
    _instructions[0xFD] = CPU::op_sbc<ABSX>;
    _instructions[0xF9] = CPU::op_sbc<ABSY>;
    _instructions[0xE1] = CPU::op_sbc<INDX>;
    _instructions[0xF1] = CPU::op_sbc<INDY>;

    _instructions[0x38] = CPU::op_sec;
    _instructions[0xF8] = CPU::op_sed;
    _instructions[0x78] = CPU::op_sei;

    _instructions[0x85] = CPU::op_sta<ZP>;
    _instructions[0x95] = CPU::op_sta<ZPX>;
    _instructions[0x8D] = CPU::op_sta<ABS>;
    _instructions[0x9D] = CPU::op_sta<ABSX>;
    _instructions[0x99] = CPU::op_sta<ABSY>;
    _instructions[0x81] = CPU::op_sta<INDX>;
    _instructions[0x91] = CPU::op_sta<INDY>;
    _instructions[0x86] = CPU::op_stx<ZP>;
    _instructions[0x96] = CPU::op_stx<ZPY>;
    _instructions[0x8E] = CPU::op_stx<ABS>;
    _instructions[0x84] = CPU::op_sty<ZP>;
    _instructions[0x94] = CPU::op_sty<ZPX>;
    _instructions[0x8C] = CPU::op_sty<ABS>;

    _instructions[0xAA] = CPU::op_tax;
    _instructions[0xA8] = CPU::op_tay;
    _instructions[0xBA] = CPU::op_tsx;
    _instructions[0x8A] = CPU::op_txa;
    _instructions[0x9A] = CPU::op_txs;
}

template <typename AccessMode>
cpu_cycle_t CPU::op_adc()
{
    AccessMode am(_registers, _memory.get());
    addToA(am.read());
    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_and()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = operand & _registers.A;

    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.A = result;

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_asl()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = operand << 1;

    _registers.setFlag(Registers::Flags::C, operand & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.A = result;

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_bit()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = _registers.A & operand;

    _registers.setFlag(Registers::Flags::N, result & 0b10000000);
    _registers.setFlag(Registers::Flags::V, result & 0b01000000);
    _registers.setFlag(Registers::Flags::Z, result == 0);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_cmp()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = _registers.A - operand;

    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::C, _registers.A >= operand );

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_cpx()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = _registers.X - operand;

    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::C, _registers.X >= operand);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_cpy()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = _registers.Y - operand;

    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.setFlag(Registers::Flags::C, _registers.Y >= operand);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_dec()
{
    AccessMode am(_registers, _memory.get());
    int8_t operand = am.read();

    operand--;
    _registers.setFlag(Registers::Flags::N, operand & 0x80);
    _registers.setFlag(Registers::Flags::Z, operand == 0);
    am.write(operand);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_eor()
{
    AccessMode am(_registers, _memory.get());
    int8_t operand = am.read();
    int8_t result = _registers.A ^ operand;

    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.A = result;

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_inc()
{
    AccessMode am(_registers, _memory.get());
    int8_t operand = am.read();

    operand++;
    _registers.setFlag(Registers::Flags::N, operand & 0x80);
    _registers.setFlag(Registers::Flags::Z, operand == 0);
    am.write(operand);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_lda()
{
    AccessMode am(_registers, _memory.get());
    _registers.A = am.read();
    _registers.setFlag(Registers::Flags::N, _registers.A & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.A == 0);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_ldx()
{
    AccessMode am(_registers, _memory.get());
    _registers.X = am.read();
    _registers.setFlag(Registers::Flags::N, _registers.X & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.X == 0);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_ldy()
{
    AccessMode am(_registers, _memory.get());
    _registers.Y = am.read();
    _registers.setFlag(Registers::Flags::N, _registers.Y & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.Y == 0);

    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_lsr()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = operand >> 1;

    _registers.setFlag(Registers::Flags::C, operand & 1);
    _registers.setFlag(Registers::Flags::N, false);;
    _registers.setFlag(Registers::Flags::Z, result == 0);

    am.write(result);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_ora()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = _registers.A | operand;

    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);
    _registers.A = result;

    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_rol()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = (operand << 1) | (_registers.P & Registers::Flags::C);

    _registers.setFlag(Registers::Flags::C, operand >> 7);
    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);

    am.write(result);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_ror()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = (operand >> 1) | ((_registers.P & Registers::Flags::C) << 7);

    _registers.setFlag(Registers::Flags::C, operand & 0x01);
    _registers.setFlag(Registers::Flags::N, result & 0x80);
    _registers.setFlag(Registers::Flags::Z, result == 0);

    am.write(result);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_sbc()
{
    AccessMode am(_registers, _memory.get());
    addToA(~am.read());
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_sta()
{
    AccessMode am(_registers, _memory.get());
    am.write(_registers.A);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_stx()
{
    AccessMode am(_registers, _memory.get());
    am.write(_registers.X);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_sty()
{
    AccessMode am(_registers, _memory.get());
    am.write(_registers.Y);
    return am.getCycles();
}

cpu_cycle_t CPU::op_inx()
{
    _registers.X++;
    _registers.setFlag(Registers::Flags::N, _registers.X & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.X == 0);
    return 1;
}

cpu_cycle_t CPU::op_iny()
{
    _registers.Y++;
    _registers.setFlag(Registers::Flags::N, _registers.Y & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.Y == 0);
    return 1;
}

cpu_cycle_t CPU::op_jmp_abs()
{
    auto address = _memory->readShort(_registers.PC);
    _registers.PC = address;

    return 2;
}

cpu_cycle_t CPU::op_jmp_ind()
{
    auto address = _memory->readShort(_registers.PC);
    _registers.PC = _memory->readShort(address);

    return 5;
}

cpu_cycle_t CPU::op_jsr()
{
    _memory->writeShortToStack(_registers.S, _registers.PC + 1);
    _registers.S -= 2;
    _registers.PC = _memory->readShort(_registers.PC);

    return 6;
}

cpu_cycle_t CPU::op_bcc()
{
    return branchOnFlag(Registers::Flags::C, false);
}

cpu_cycle_t CPU::op_bcs()
{
    return branchOnFlag(Registers::Flags::C, true);
}

cpu_cycle_t CPU::op_beq()
{
    return branchOnFlag(Registers::Flags::Z, true);
}

cpu_cycle_t CPU::op_bmi()
{
    return branchOnFlag(Registers::Flags::N, true);
}

cpu_cycle_t CPU::op_bne()
{
    return branchOnFlag(Registers::Flags::Z, false);
}

cpu_cycle_t CPU::op_bpl()
{
    return branchOnFlag(Registers::Flags::N, false);
}

cpu_cycle_t CPU::op_brk()
{
    _registers.PC = _memory->readBrkVector();
    _registers.setFlag(Registers::Flags::I, true);
    return 6;
}

cpu_cycle_t CPU::op_bvc()
{
    return branchOnFlag(Registers::Flags::V, false);
}

cpu_cycle_t CPU::op_bvs()
{
    return branchOnFlag(Registers::Flags::V, true);
}

cpu_cycle_t CPU::op_clc()
{
    _registers.setFlag(Registers::Flags::C, false);
    return 1;
}

cpu_cycle_t CPU::op_cld()
{
    _registers.setFlag(Registers::Flags::D, false);
    return 1;
}

cpu_cycle_t CPU::op_cli()
{
    _registers.setFlag(Registers::Flags::I, false);
    return 1;
}

cpu_cycle_t CPU::op_clv()
{
    _registers.setFlag(Registers::Flags::V, false);
    return 1;
}

cpu_cycle_t CPU::op_dex()
{
    _registers.X--;
    _registers.setFlag(Registers::Flags::N, _registers.X & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.X == 0);
    return 1;
}

cpu_cycle_t CPU::op_dey()
{
    _registers.Y--;
    _registers.setFlag(Registers::Flags::N, _registers.Y & 0x80);
    _registers.setFlag(Registers::Flags::Z, _registers.Y == 0);
    return 1;
}

cpu_cycle_t CPU::op_nop()
{
    return 1;
}

cpu_cycle_t CPU::op_pha()
{
    _memory->writeByteToStack(_registers.S--, _registers.A);
    return 2;
}

cpu_cycle_t CPU::op_php()
{
    _memory->writeByteToStack(_registers.S--, _registers.P);
    return 2;
}

cpu_cycle_t CPU::op_pla()
{
    _registers.A = _memory->readByteFromStack(++_registers.S);
    return 3;
}

cpu_cycle_t CPU::op_plp()
{
    _registers.P = _memory->readShortFromStack(++_registers.S);
    return 3;
}

cpu_cycle_t CPU::op_rti()
{
    _registers.S++;
    _registers.P = _memory->readByteFromStack(_registers.S++);
    _registers.PC = _memory->readShortFromStack(_registers.S);
    _registers.S += 2;
    return 5;
}

cpu_cycle_t CPU::op_rts()
{
    _registers.S++;
    _registers.PC = _memory->readShortFromStack(_registers.S) + 1;
    _registers.S += 2;
    return 5;
}

cpu_cycle_t CPU::op_sec()
{
    _registers.setFlag(Registers::Flags::C, true);
    return 1;
}

cpu_cycle_t CPU::op_sed()
{
    _registers.setFlag(Registers::Flags::D, true);
    return 1;
}

cpu_cycle_t CPU::op_sei()
{
    _registers.setFlag(Registers::Flags::I, true);
    return 1;
}

cpu_cycle_t CPU::op_tax()
{
    _registers.X = _registers.A;
    return 1;
}

cpu_cycle_t CPU::op_tay()
{
    _registers.Y = _registers.A;
    return 1;
}

cpu_cycle_t CPU::op_tsx()
{
    _registers.X = _registers.S;
    return 1;
}

cpu_cycle_t CPU::op_txa()
{
    _registers.A = _registers.X;
    return 1;
}

cpu_cycle_t CPU::op_txs()
{
    _registers.S = _registers.X;
    return 1;
}

cpu_cycle_t CPU::branchOnFlag(CPU::Registers::Flags flag, bool state)
{
    if (_registers.getFlag(flag) == state)
    {
        auto jump = _registers.PC + _memory->readByte(_registers.PC) - 1;
        auto page = _registers.PC & 0xFF00;
        auto jumpPage = jump & 0xFF00;
        _registers.PC = jump;

        return page == jumpPage ? 1 : 2;
    }
    else
    {
        _registers.PC++;
    }

    return 1;
}

void CPU::addToA(uint8_t value)
{
    uint16_t result = _registers.A + value;
    if (_registers.getFlag(Registers::Flags::C))
    {
        result++;
    }

    _registers.setFlag(Registers::Flags::C, result > 0xFF);
    _registers.setFlag(Registers::Flags::V, ~(_registers.A ^ value) & (_registers.A ^ result) & 0x80);
    _registers.setFlag(Registers::Flags::Z, static_cast<uint8_t>(result) == 0);
    _registers.setFlag(Registers::Flags::N, static_cast<uint8_t>(result) & 0x80);
    _registers.A = static_cast<uint8_t>(result);
}

}
