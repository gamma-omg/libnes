#include <cstdint>
#include <stdexcept>
#include <memory.h>
#include <functional>
#include "CPU.h"
#include "CPUMemory.h"

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
{
    reset();
}

void CPU::Registers::reset()
{
    A = X = Y = 0;
    P = Flags::I | Flags::B;
    S = 0xFF;
    PC = 0;
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


CPU::CPU()
    : _memory(std::make_shared<CPUMemory>())
    , _cycle(0)
    , _dmaCycle(0)
{
   _registers.reset();
    setupInstructions();
}

CPU::CPU(const std::vector<uint8_t>& operations)
    : CPU()
{
    _memory->loadProgram(operations);
    _memory->setResetVector(CPUMemory::ROM_OFFSET);
    reset();
}

void CPU::reset()
{
    _killed = false;
    _registers.reset();
    _registers.PC = _memory->readShort(CPUMemory::RESET_VECTOR);
}

void CPU::tick()
{
    if (_killed)
    {
        return;
    }

    if (_dmaCycle > 0)
    {
        _dmaCycle--;
        return;
    }

    auto opcode = _memory->readByte(_registers.PC++);
    auto handler = _instructions[opcode];

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

void CPU::startDmaTransfer()
{
    _dmaCycle = _cycle % 2 == 0 ? 513 : 514;
}

CPU::Registers &CPU::getRegisters()
{
    return _registers;
}

std::shared_ptr<CPUMemory> CPU::getMemory()
{
    return _memory;
}

cpu_cycle_t CPU::getCycle() const
{
    return _cycle;
}

void CPU::setupInstructions()
{
    memset(&_instructions, 0, sizeof(InstructionHandler) * 0x100);

    _instructions[0x69] = &CPU::op_adc<IMM>;
    _instructions[0x65] = &CPU::op_adc<ZP>;
    _instructions[0x75] = &CPU::op_adc<ZPX>;
    _instructions[0x6D] = &CPU::op_adc<ABS>;
    _instructions[0x7D] = &CPU::op_adc<ABSX>;
    _instructions[0x79] = &CPU::op_adc<ABSY>;
    _instructions[0x61] = &CPU::op_adc<INDX>;
    _instructions[0x71] = &CPU::op_adc<INDY>;
    
    _instructions[0x29] = &CPU::op_and<IMM>;
    _instructions[0x25] = &CPU::op_and<ZP>;
    _instructions[0x35] = &CPU::op_and<ZPX>;
    _instructions[0x2D] = &CPU::op_and<ABS>;
    _instructions[0x3D] = &CPU::op_and<ABSX>;
    _instructions[0x39] = &CPU::op_and<ABSY>;
    _instructions[0x21] = &CPU::op_and<INDX>;
    _instructions[0x31] = &CPU::op_and<INDY>;

    _instructions[0x0A] = &CPU::op_asl<ACC>;
    _instructions[0x06] = &CPU::op_asl<ZP>;
    _instructions[0x16] = &CPU::op_asl<ZPX>;
    _instructions[0x0E] = &CPU::op_asl<ABS>;
    _instructions[0x1E] = &CPU::op_asl<ABSX>;

    _instructions[0x90] = &CPU::op_bcc;
    _instructions[0xB0] = &CPU::op_bcs;
    _instructions[0xF0] = &CPU::op_beq;
    _instructions[0x30] = &CPU::op_bmi;
    _instructions[0xD0] = &CPU::op_bne;
    _instructions[0x10] = &CPU::op_bpl;
    _instructions[0x50] = &CPU::op_bvc;
    _instructions[0x70] = &CPU::op_bvs;

    _instructions[0x24] = &CPU::op_bit<ZP>;
    _instructions[0x2C] = &CPU::op_bit<ABS>;

    _instructions[0x00] = &CPU::op_brk;

    _instructions[0x18] = &CPU::op_clc;
    _instructions[0xD8] = &CPU::op_cld;
    _instructions[0x58] = &CPU::op_cli;
    _instructions[0xB8] = &CPU::op_clv;

    _instructions[0xC9] = &CPU::op_cmp<IMM>;
    _instructions[0xC5] = &CPU::op_cmp<ZP>;
    _instructions[0xD5] = &CPU::op_cmp<ZPX>;
    _instructions[0xCD] = &CPU::op_cmp<ABS>;
    _instructions[0xDD] = &CPU::op_cmp<ABSX>;
    _instructions[0xD9] = &CPU::op_cmp<ABSY>;
    _instructions[0xC1] = &CPU::op_cmp<INDX>;
    _instructions[0xD1] = &CPU::op_cmp<INDY>;

    _instructions[0xE0] = &CPU::op_cpx<IMM>;
    _instructions[0xE4] = &CPU::op_cpx<ZP>;
    _instructions[0xEC] = &CPU::op_cpx<ABS>;
    _instructions[0xC0] = &CPU::op_cpy<IMM>;
    _instructions[0xC4] = &CPU::op_cpy<ZP>;
    _instructions[0xCC] = &CPU::op_cpy<ABS>;

    _instructions[0xC6] = &CPU::op_dec<ZP>;
    _instructions[0xD6] = &CPU::op_dec<ZPX>;
    _instructions[0xCE] = &CPU::op_dec<ABS>;
    _instructions[0xDE] = &CPU::op_dec<ABSX>;
    _instructions[0xCA] = &CPU::op_dex;
    _instructions[0x88] = &CPU::op_dey;

    _instructions[0x49] = &CPU::op_eor<IMM>;
    _instructions[0x45] = &CPU::op_eor<ZP>;
    _instructions[0x55] = &CPU::op_eor<ZPX>;
    _instructions[0x4D] = &CPU::op_eor<ABS>;
    _instructions[0x5D] = &CPU::op_eor<ABSX>;
    _instructions[0x59] = &CPU::op_eor<ABSY>;
    _instructions[0x41] = &CPU::op_eor<INDX>;
    _instructions[0x51] = &CPU::op_eor<INDY>;

    _instructions[0xE6] = &CPU::op_inc<ZP>;
    _instructions[0xF6] = &CPU::op_inc<ZPX>;
    _instructions[0xEE] = &CPU::op_inc<ABS>;
    _instructions[0xFE] = &CPU::op_inc<ABSX>;
    _instructions[0xE8] = &CPU::op_inx;
    _instructions[0xC8] = &CPU::op_iny;

    _instructions[0x4C] = &CPU::op_jmp_abs;
    _instructions[0x6C] = &CPU::op_jmp_ind;
    _instructions[0x20] = &CPU::op_jsr;

    _instructions[0xA9] = &CPU::op_lda<IMM>;
    _instructions[0xA5] = &CPU::op_lda<ZP>;
    _instructions[0xB5] = &CPU::op_lda<ZPX>;
    _instructions[0xAD] = &CPU::op_lda<ABS>;
    _instructions[0xBD] = &CPU::op_lda<ABSX>;
    _instructions[0xB9] = &CPU::op_lda<ABSY>;
    _instructions[0xA1] = &CPU::op_lda<INDX>;
    _instructions[0xB1] = &CPU::op_lda<INDY>;
    _instructions[0xA2] = &CPU::op_ldx<IMM>;
    _instructions[0xA6] = &CPU::op_ldx<ZP>;
    _instructions[0xB6] = &CPU::op_ldx<ZPY>;
    _instructions[0xAE] = &CPU::op_ldx<ABS>;
    _instructions[0xBE] = &CPU::op_ldx<ABSY>;
    _instructions[0xA0] = &CPU::op_ldy<IMM>;
    _instructions[0xA4] = &CPU::op_ldy<ZP>;
    _instructions[0xB4] = &CPU::op_ldy<ZPX>;
    _instructions[0xAC] = &CPU::op_ldy<ABS>;
    _instructions[0xBC] = &CPU::op_ldy<ABSX>;

    _instructions[0x4A] = &CPU::op_lsr<ACC>;
    _instructions[0x46] = &CPU::op_lsr<ZP>;
    _instructions[0x56] = &CPU::op_lsr<ZPX>;
    _instructions[0x4E] = &CPU::op_lsr<ABS>;
    _instructions[0x5E] = &CPU::op_lsr<ABSX>;

    _instructions[0xEA] = &CPU::op_nop;

    _instructions[0x09] = &CPU::op_ora<IMM>;
    _instructions[0x05] = &CPU::op_ora<ZP>;
    _instructions[0x15] = &CPU::op_ora<ZPX>;
    _instructions[0x0D] = &CPU::op_ora<ABS>;
    _instructions[0x1D] = &CPU::op_ora<ABSX>;
    _instructions[0x19] = &CPU::op_ora<ABSY>;
    _instructions[0x01] = &CPU::op_ora<INDX>;
    _instructions[0x11] = &CPU::op_ora<INDY>;

    _instructions[0x48] = &CPU::op_pha;
    _instructions[0x08] = &CPU::op_php;
    _instructions[0x68] = &CPU::op_pla;
    _instructions[0x28] = &CPU::op_plp;

    _instructions[0x2A] = &CPU::op_rol<ACC>;
    _instructions[0x26] = &CPU::op_rol<ZP>;
    _instructions[0x36] = &CPU::op_rol<ZPX>;
    _instructions[0x2E] = &CPU::op_rol<ABS>;
    _instructions[0x3E] = &CPU::op_rol<ABSX>;
    _instructions[0x6A] = &CPU::op_ror<ACC>;
    _instructions[0x66] = &CPU::op_ror<ZP>;
    _instructions[0x76] = &CPU::op_ror<ZPX>;
    _instructions[0x6E] = &CPU::op_ror<ABS>;
    _instructions[0x7E] = &CPU::op_ror<ABSX>;

    _instructions[0x40] = &CPU::op_rti;
    _instructions[0x60] = &CPU::op_rts;

    _instructions[0xE9] = &CPU::op_sbc<IMM>;
    _instructions[0xEB] = &CPU::op_sbc<IMM>;
    _instructions[0xE5] = &CPU::op_sbc<ZP>;
    _instructions[0xF5] = &CPU::op_sbc<ZPX>;
    _instructions[0xED] = &CPU::op_sbc<ABS>;
    _instructions[0xFD] = &CPU::op_sbc<ABSX>;
    _instructions[0xF9] = &CPU::op_sbc<ABSY>;
    _instructions[0xE1] = &CPU::op_sbc<INDX>;
    _instructions[0xF1] = &CPU::op_sbc<INDY>;

    _instructions[0x38] = &CPU::op_sec;
    _instructions[0xF8] = &CPU::op_sed;
    _instructions[0x78] = &CPU::op_sei;

    _instructions[0x85] = &CPU::op_sta<ZP>;
    _instructions[0x95] = &CPU::op_sta<ZPX>;
    _instructions[0x8D] = &CPU::op_sta<ABS>;
    _instructions[0x9D] = &CPU::op_sta<ABSX>;
    _instructions[0x99] = &CPU::op_sta<ABSY>;
    _instructions[0x81] = &CPU::op_sta<INDX>;
    _instructions[0x91] = &CPU::op_sta<INDY>;
    _instructions[0x86] = &CPU::op_stx<ZP>;
    _instructions[0x96] = &CPU::op_stx<ZPY>;
    _instructions[0x8E] = &CPU::op_stx<ABS>;
    _instructions[0x84] = &CPU::op_sty<ZP>;
    _instructions[0x94] = &CPU::op_sty<ZPX>;
    _instructions[0x8C] = &CPU::op_sty<ABS>;

    _instructions[0xAA] = &CPU::op_tax;
    _instructions[0xA8] = &CPU::op_tay;
    _instructions[0xBA] = &CPU::op_tsx;
    _instructions[0x8A] = &CPU::op_txa;
    _instructions[0x9A] = &CPU::op_txs;
    _instructions[0x98] = &CPU::op_tya;

    _instructions[0x4B] = &CPU::op_alr;
    _instructions[0x0B] = &CPU::op_anc;
    _instructions[0x2B] = &CPU::op_anc;
    _instructions[0x6B] = &CPU::op_arr;
    _instructions[0xCB] = &CPU::op_axs;

    _instructions[0xAB] = &CPU::op_lax<IMM>;
    _instructions[0xA7] = &CPU::op_lax<ZP>;
    _instructions[0xB7] = &CPU::op_lax<ZPY>;
    _instructions[0xAF] = &CPU::op_lax<ABS>;
    _instructions[0xBF] = &CPU::op_lax<ABSY>;
    _instructions[0xA3] = &CPU::op_lax<INDX>;
    _instructions[0xB3] = &CPU::op_lax<INDY>;

    _instructions[0x87] = &CPU::op_sax<ZP>;
    _instructions[0x97] = &CPU::op_sax<ZPY>;
    _instructions[0x83] = &CPU::op_sax<INDX>;
    _instructions[0x8F] = &CPU::op_sax<ABS>;

    _instructions[0xC7] = &CPU::op_dcp<ZP>;
    _instructions[0xD7] = &CPU::op_dcp<ZPX>;
    _instructions[0xCF] = &CPU::op_dcp<ABS>;
    _instructions[0xDF] = &CPU::op_dcp<ABSX>;
    _instructions[0xDB] = &CPU::op_dcp<ABSY>;
    _instructions[0xC3] = &CPU::op_dcp<INDX>;
    _instructions[0xD3] = &CPU::op_dcp<INDY>;

    _instructions[0xE7] = &CPU::op_isc<ZP>;
    _instructions[0xF7] = &CPU::op_isc<ZPX>;
    _instructions[0xEF] = &CPU::op_isc<ABS>;
    _instructions[0xFF] = &CPU::op_isc<ABSX>;
    _instructions[0xFB] = &CPU::op_isc<ABSY>;
    _instructions[0xE3] = &CPU::op_isc<INDX>;
    _instructions[0xF3] = &CPU::op_isc<INDY>;

    _instructions[0x27] = &CPU::op_rla<ZP>;
    _instructions[0x37] = &CPU::op_rla<ZPX>;
    _instructions[0x2F] = &CPU::op_rla<ABS>;
    _instructions[0x3F] = &CPU::op_rla<ABSX>;
    _instructions[0x3B] = &CPU::op_rla<ABSY>;
    _instructions[0x23] = &CPU::op_rla<INDX>;
    _instructions[0x33] = &CPU::op_rla<INDY>;

    _instructions[0x67] = &CPU::op_rra<ZP>;
    _instructions[0x77] = &CPU::op_rra<ZPX>;
    _instructions[0x6F] = &CPU::op_rra<ABS>;
    _instructions[0x7F] = &CPU::op_rra<ABSX>;
    _instructions[0x7B] = &CPU::op_rra<ABSY>;
    _instructions[0x63] = &CPU::op_rra<INDX>;
    _instructions[0x73] = &CPU::op_rra<INDY>;

    _instructions[0x07] = &CPU::op_slo<ZP>;
    _instructions[0x17] = &CPU::op_slo<ZPX>;
    _instructions[0x0F] = &CPU::op_slo<ABS>;
    _instructions[0x1F] = &CPU::op_slo<ABSX>;
    _instructions[0x1B] = &CPU::op_slo<ABSY>;
    _instructions[0x03] = &CPU::op_slo<INDX>;
    _instructions[0x13] = &CPU::op_slo<INDY>;

    _instructions[0x47] = &CPU::op_sre<ZP>;
    _instructions[0x57] = &CPU::op_sre<ZPX>;
    _instructions[0x4F] = &CPU::op_sre<ABS>;
    _instructions[0x5F] = &CPU::op_sre<ABSX>;
    _instructions[0x5B] = &CPU::op_sre<ABSY>;
    _instructions[0x43] = &CPU::op_sre<INDX>;
    _instructions[0x53] = &CPU::op_sre<INDY>;

    _instructions[0xEB] = &CPU::op_sbc<IMM>;

    _instructions[0x1A] = &CPU::op_nop;
    _instructions[0x3A] = &CPU::op_nop;
    _instructions[0x5A] = &CPU::op_nop;
    _instructions[0x7A] = &CPU::op_nop;
    _instructions[0xDA] = &CPU::op_nop;
    _instructions[0xFA] = &CPU::op_nop;

    _instructions[0x04] = &CPU::op_skb<ZP>;
    _instructions[0x14] = &CPU::op_skb<ZPX>;
    _instructions[0x34] = &CPU::op_skb<ZPX>;
    _instructions[0x44] = &CPU::op_skb<ZP>;
    _instructions[0x54] = &CPU::op_skb<ZPX>;
    _instructions[0x64] = &CPU::op_skb<ZP>;
    _instructions[0x74] = &CPU::op_skb<ZPX>;
    _instructions[0x80] = &CPU::op_skb<IMM>;
    _instructions[0x82] = &CPU::op_skb<IMM>;
    _instructions[0x89] = &CPU::op_skb<IMM>;
    _instructions[0xC2] = &CPU::op_skb<IMM>;
    _instructions[0xD4] = &CPU::op_skb<ZPX>;
    _instructions[0xE2] = &CPU::op_skb<IMM>;
    _instructions[0xF4] = &CPU::op_skb<ZPX>;

    _instructions[0x0C] = &CPU::op_ign<ABS>;
    _instructions[0x1C] = &CPU::op_ign<ABSX>;
    _instructions[0x3C] = &CPU::op_ign<ABSX>;
    _instructions[0x5C] = &CPU::op_ign<ABSX>;
    _instructions[0x7C] = &CPU::op_ign<ABSX>;
    _instructions[0xDC] = &CPU::op_ign<ABSX>;
    _instructions[0xFC] = &CPU::op_ign<ABSX>;

    _instructions[0x9F] = &CPU::op_axa<ABSY>;
    _instructions[0x93] = &CPU::op_axa<INDY>;

    _instructions[0x02] = &CPU::op_kil;
    _instructions[0x12] = &CPU::op_kil;
    _instructions[0x22] = &CPU::op_kil;
    _instructions[0x32] = &CPU::op_kil;
    _instructions[0x42] = &CPU::op_kil;
    _instructions[0x52] = &CPU::op_kil;
    _instructions[0x62] = &CPU::op_kil;
    _instructions[0x72] = &CPU::op_kil;
    _instructions[0x92] = &CPU::op_kil;
    _instructions[0xB2] = &CPU::op_kil;
    _instructions[0xD2] = &CPU::op_kil;
    _instructions[0xF2] = &CPU::op_kil;

    _instructions[0xBB] = &CPU::op_las;
    _instructions[0x9E] = &CPU::op_sxa;
    _instructions[0x9C] = &CPU::op_sya;
    _instructions[0x8B] = &CPU::op_xaa;
    _instructions[0x9B] = &CPU::op_xas;
}

template <typename AccessMode>
cpu_cycle_t CPU::op_adc()
{
    AccessMode am(_registers, _memory.get());
    _registers.A = _add(am.read());
    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_and()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    _registers.A = _and(operand, _registers.A);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_asl()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    am.write(_asl(operand));
    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_bit()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = _registers.A & operand;

    _registers.setFlag(Registers::Flags::N, operand & 0x80);
    _registers.setFlag(Registers::Flags::V, operand & 0x40);
    _registers.setFlag(Registers::Flags::Z, result == 0);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_cmp()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t diff = _registers.A - operand;

    updateZNFlags(diff);
    _registers.setFlag(Registers::Flags::C, _registers.A >= operand );

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_cpx()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = _registers.X - operand;

    updateZNFlags(result);
    _registers.setFlag(Registers::Flags::C, _registers.X >= operand);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_cpy()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint8_t result = _registers.Y - operand;

    updateZNFlags(result);
    _registers.setFlag(Registers::Flags::C, _registers.Y >= operand);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_dec()
{
    AccessMode am(_registers, _memory.get());
    int8_t operand = am.read();

    operand--;
    updateZNFlags(operand);
    am.write(operand);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_eor()
{
    AccessMode am(_registers, _memory.get());
    _registers.A = _eor(_registers.A, am.read());
    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_inc()
{
    AccessMode am(_registers, _memory.get());
    int8_t operand = am.read();

    operand++;
    updateZNFlags(operand);
    am.write(operand);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_lda()
{
    AccessMode am(_registers, _memory.get());
    _registers.A = am.read();
    updateZNFlags(_registers.A);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_ldx()
{
    AccessMode am(_registers, _memory.get());
    _registers.X = am.read();
    updateZNFlags(_registers.X);

    return am.getCycles();
}

template <typename AccessMode>
cpu_cycle_t CPU::op_ldy()
{
    AccessMode am(_registers, _memory.get());
    _registers.Y = am.read();
    updateZNFlags(_registers.Y);

    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_lsr()
{
    AccessMode am(_registers, _memory.get());
    am.write(_lsr(am.read()));
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_ora()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    _registers.A = _or(_registers.A, operand);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_rol()
{
    AccessMode am(_registers, _memory.get());
    am.write(_rol(am.read()));
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_ror()
{
    AccessMode am(_registers, _memory.get());
    am.write(_ror(am.read()));
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_sbc()
{
    AccessMode am(_registers, _memory.get());
    _registers.A = _add(~am.read());
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

template<typename AccessMode>
cpu_cycle_t CPU::op_lax()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    _registers.A = operand;
    _registers.X = operand;
    updateZNFlags(operand);

    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_sax()
{
    AccessMode am(_registers, _memory.get());
    am.write(_registers.A & _registers.X);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_dcp()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    operand--;

    uint8_t diff = _registers.A - operand;
    updateZNFlags(diff);
    _registers.setFlag(Registers::Flags::C, _registers.A >= operand);

    am.write(operand);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_isc()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    operand++;

    _registers.A = _add(~operand);
    am.write(operand);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_rla()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    operand = _rol(operand);
    _registers.A = _and(_registers.A, operand);

    am.write(operand);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_rra()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    operand = _ror(operand);
    _registers.A = _add(operand);

    am.write(operand);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_slo()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    operand = _asl(operand);
    _registers.A = _or(_registers.A, operand);

    am.write(operand);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_sre()
{
    AccessMode am(_registers, _memory.get());
    uint8_t operand = am.read();
    operand = _lsr(operand);
    _registers.A = _eor(_registers.A, operand);

    am.write(operand);
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_skb()
{
    AccessMode am(_registers, _memory.get());
    am.read();
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_ign()
{
    AccessMode am(_registers, _memory.get());
    am.read();
    return am.getCycles();
}

template<typename AccessMode>
cpu_cycle_t CPU::op_axa()
{
    AccessMode am(_registers, _memory.get());
    uint8_t result = _registers.X & _registers.A;
    result &= 7;
    am.write(result);

    return am.getCycles();
}

cpu_cycle_t CPU::op_inx()
{
    _registers.X++;
    updateZNFlags(_registers.X);
    return 1;
}

cpu_cycle_t CPU::op_iny()
{
    _registers.Y++;
    updateZNFlags(_registers.Y);
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
    uint16_t base = _memory->readShort(_registers.PC);
    uint16_t address = 0;
    if ((base & 0xFF) == 0xFF)
    {
        uint8_t l = _memory->readByte(base);
        uint8_t h = _memory->readByte(base - 0xFF);
        address = l | (h << 8);
    }
    else
    {
        address = _memory->readShort(base);
    }

    _registers.PC = address;
    return 5;
}

cpu_cycle_t CPU::op_jsr()
{
    _memory->pushShort(_registers.S, _registers.PC + 1);
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
    _memory->pushShort(_registers.S, _registers.PC + 1);
    _memory->pushByte(_registers.S, _registers.P);
    _registers.PC = _memory->readShort(CPUMemory::IRQ_VECTOR);
    _registers.setFlag(Registers::Flags::I, true);
    _registers.setFlag(Registers::Flags::D, true);
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
    updateZNFlags(_registers.X);
    return 1;
}

cpu_cycle_t CPU::op_dey()
{
    _registers.Y--;
    updateZNFlags(_registers.Y);
    return 1;
}

cpu_cycle_t CPU::op_nop()
{
    return 1;
}

cpu_cycle_t CPU::op_pha()
{
    _memory->pushByte(_registers.S, _registers.A);
    return 2;
}

cpu_cycle_t CPU::op_php()
{
    _memory->pushByte(_registers.S, _registers.P);
    return 2;
}

cpu_cycle_t CPU::op_pla()
{
    _registers.A = _memory->popByte(_registers.S);
    updateZNFlags(_registers.A);
    return 3;
}

cpu_cycle_t CPU::op_plp()
{
    _registers.P = _memory->popByte(_registers.S);
    _registers.P |= Registers::Flags::B | Registers::Flags::L;
    return 3;
}

cpu_cycle_t CPU::op_rti()
{
    _registers.P = _memory->popByte(_registers.S);
    _registers.P |= Registers::Flags::B | Registers::Flags::L;
    _registers.PC = _memory->popShort(_registers.S);
    return 5;
}

cpu_cycle_t CPU::op_rts()
{
    _registers.PC = _memory->popShort(_registers.S) + 1;
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
    updateZNFlags(_registers.X);
    return 1;
}

cpu_cycle_t CPU::op_tay()
{
    _registers.Y = _registers.A;
    updateZNFlags(_registers.Y);
    return 1;
}

cpu_cycle_t CPU::op_tsx()
{
    _registers.X = _registers.S;
    updateZNFlags(_registers.X);
    return 1;
}

cpu_cycle_t CPU::op_txa()
{
    _registers.A = _registers.X;
    updateZNFlags(_registers.A);
    return 1;
}

cpu_cycle_t CPU::op_tya()
{
    _registers.A = _registers.Y;
    updateZNFlags(_registers.A);
    return 1;
}

cpu_cycle_t CPU::op_txs()
{
    _registers.S = _registers.X;
    return 1;
}

cpu_cycle_t CPU::op_alr()
{
    IMM am(_registers, _memory.get());
    _registers.A = _and(_registers.A, am.read());
    _registers.A = _lsr(_registers.A);

    return 1;
}

cpu_cycle_t CPU::op_anc()
{
    IMM am(_registers, _memory.get());
    _registers.A = _and(am.read(), _registers.A);
    _registers.setFlag(Registers::Flags::C, _registers.getFlag(Registers::Flags::N));
    return 1;
}

cpu_cycle_t CPU::op_arr()
{
    IMM am(_registers, _memory.get());
    _registers.A = _and(am.read(), _registers.A);
    _registers.A = _ror(_registers.A);

    uint8_t bit6 = (_registers.A & 0x40) >> 5;
    uint8_t bit5 = (_registers.A & 0x20) >> 4;

    _registers.setFlag(Registers::Flags::C, bit6);
    _registers.setFlag(Registers::Flags::V, bit6 ^ bit5);

    return 1;
}

cpu_cycle_t CPU::op_axs()
{
    IMM am(_registers, _memory.get());
    uint8_t operand = am.read();
    uint16_t result = (_registers.A & _registers.X) - operand;

    _registers.X = result & 0xFF;
    _registers.setFlag(Registers::Flags::C, (result & 0x100) == 0);
    updateZNFlags(_registers.X);

    return 1;
}

cpu_cycle_t CPU::op_kil()
{
    _killed = true;
    return 0;
}

cpu_cycle_t CPU::op_las()
{
    ABSY am(_registers, _memory.get());
    uint8_t result = _and(am.read(), _registers.S);
    _registers.A = result;
    _registers.X = result;
    _registers.S = result;

    return am.getCycles();
}

cpu_cycle_t CPU::op_sxa()
{
    uint16_t address = _memory->readShort(_registers.PC) + _registers.Y;
    _registers.PC += 2;

    uint8_t al = address & 0xFF;
    uint8_t ah = address >> 8;
    uint8_t result = _registers.X & (ah + 1);

    _memory->writeByte((result << 8) | al, result);
    return 4;
}

cpu_cycle_t CPU::op_sya()
{
    uint16_t address = _memory->readShort(_registers.PC) + _registers.X;
    _registers.PC += 2;

    uint8_t al = address & 0xFF;
    uint8_t ah = address >> 8;
    uint8_t result = _registers.Y & (ah + 1);

    _memory->writeByte((result << 8) | al, result);
    return 4;
}

cpu_cycle_t CPU::op_xaa()
{
    IMM am(_registers, _memory.get());
    am.read();

    // exact operation unknown
    return am.getCycles();
}

cpu_cycle_t CPU::op_xas()
{
    ABSY am(_registers, _memory.get());
    uint16_t operand = am.getAddress() >> 8;
    operand++;

    _registers.S = _registers.X & _registers.A;
    am.write(_registers.S & operand);

    return am.getCycles();
}

cpu_cycle_t CPU::branchOnFlag(CPU::Registers::Flags flag, bool state)
{
    if (_registers.getFlag(flag) == state)
    {
        auto offset = static_cast<int8_t>(_memory->readByte(_registers.PC++));
        auto jump = _registers.PC + offset;
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

uint8_t CPU::_add(uint8_t value)
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
    return static_cast<uint8_t>(result);
}

uint8_t CPU::_and(uint8_t a, uint8_t b)
{
    uint8_t result = a & b;
    updateZNFlags(result);
    return result;
}

uint8_t CPU::_eor(uint8_t a, uint8_t b)
{
    uint8_t result = a ^ b;
    updateZNFlags(result);
    return result;
}

uint8_t CPU::_or(uint8_t a, uint8_t b)
{
    uint8_t result = a | b;
    updateZNFlags(result);
    return result;
}

uint8_t CPU::_lsr(uint8_t value)
{
    uint8_t result = value >> 1;

    _registers.setFlag(Registers::Flags::C, value & 1);
    _registers.setFlag(Registers::Flags::N, false);;
    _registers.setFlag(Registers::Flags::Z, result == 0);

    return result;
}

uint8_t CPU::_asl(uint8_t value)
{
    uint8_t result = value << 1;
    updateZNFlags(result);
    _registers.setFlag(Registers::Flags::C, value & 0x80);

    return result;
}

uint8_t CPU::_ror(uint8_t value)
{
    uint8_t result = (value >> 1) | ((_registers.P & Registers::Flags::C) << 7);

    _registers.setFlag(Registers::Flags::C, value & 0x01);
    updateZNFlags(result);
    return result;
}

uint8_t CPU::_rol(uint8_t value)
{
    uint8_t result = (value << 1) | (_registers.P & Registers::Flags::C);

    _registers.setFlag(Registers::Flags::C, value >> 7);
    updateZNFlags(result);
    return result;
}

void CPU::updateZNFlags(uint8_t value)
{
    _registers.setFlag(Registers::Flags::Z, value == 0);
    _registers.setFlag(Registers::Flags::N, value & 0x80);
}

}
