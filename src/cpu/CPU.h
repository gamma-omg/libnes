#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

#include <vector>
#include <memory>
#include <functional>
#include "../Timings.h"

namespace nescore
{

class CPUMemory;

using cpu_tick_t = uint8_t;

class CPU
{
private:
    using InstructionHandler = uint8_t (CPU::*)();

public:
    struct Registers
    {
        enum Flags
        {
            C = 0b00000001,
            Z = 0b00000010,
            I = 0b00000100,
            D = 0b00001000,
            B = 0b00010000,
            L = 0b00100000,
            V = 0b01000000,
            N = 0b10000000
        };

        uint8_t A;
        uint8_t X;
        uint8_t Y;
        uint8_t P;
        uint8_t S;
        uint16_t PC;

        Registers();

        void reset();
        void setFlag(Flags flag, bool value);
        bool getFlag(Flags flag) const;
    };

public:
    CPU();
    CPU(const std::vector<uint8_t>& operations);

    void reset();
    void update(master_cycle time);
    void tick();
    void tick(int count);
    void startDmaTransfer();
    Registers& getRegisters();
    std::shared_ptr<CPUMemory> getMemory();
    cpu_cycle getCycle() const;

private:
    void setupInstructions();

    template <typename AccessMode> cpu_tick_t op_adc();
    template <typename AccessMode> cpu_tick_t op_and();
    template <typename AccessMode> cpu_tick_t op_asl();
    template <typename AccessMode> cpu_tick_t op_cmp();
    template <typename AccessMode> cpu_tick_t op_cpx();
    template <typename AccessMode> cpu_tick_t op_cpy();
    template <typename AccessMode> cpu_tick_t op_dec();
    template <typename AccessMode> cpu_tick_t op_eor();
    template <typename AccessMode> cpu_tick_t op_inc();
    template <typename AccessMode> cpu_tick_t op_lda();
    template <typename AccessMode> cpu_tick_t op_ldx();
    template <typename AccessMode> cpu_tick_t op_ldy();
    template <typename AccessMode> cpu_tick_t op_bit();
    template <typename AccessMode> cpu_tick_t op_lsr();
    template <typename AccessMode> cpu_tick_t op_ora();
    template <typename AccessMode> cpu_tick_t op_rol();
    template <typename AccessMode> cpu_tick_t op_ror();
    template <typename AccessMode> cpu_tick_t op_sbc();
    template <typename AccessMode> cpu_tick_t op_sta();
    template <typename AccessMode> cpu_tick_t op_stx();
    template <typename AccessMode> cpu_tick_t op_sty();

    cpu_tick_t op_bcc();
    cpu_tick_t op_bcs();
    cpu_tick_t op_beq();
    cpu_tick_t op_bmi();
    cpu_tick_t op_bne();
    cpu_tick_t op_bpl();
    cpu_tick_t op_brk();
    cpu_tick_t op_bvc();
    cpu_tick_t op_bvs();
    cpu_tick_t op_clc();
    cpu_tick_t op_cld();
    cpu_tick_t op_cli();
    cpu_tick_t op_clv();
    cpu_tick_t op_dex();
    cpu_tick_t op_dey();
    cpu_tick_t op_inx();
    cpu_tick_t op_iny();
    cpu_tick_t op_jsr();
    cpu_tick_t op_jmp_abs();
    cpu_tick_t op_jmp_ind();
    cpu_tick_t op_nop();
    cpu_tick_t op_pha();
    cpu_tick_t op_php();
    cpu_tick_t op_pla();
    cpu_tick_t op_plp();
    cpu_tick_t op_rti();
    cpu_tick_t op_rts();
    cpu_tick_t op_sec();
    cpu_tick_t op_sed();
    cpu_tick_t op_sei();
    cpu_tick_t op_tax();
    cpu_tick_t op_tay();
    cpu_tick_t op_tsx();
    cpu_tick_t op_txa();
    cpu_tick_t op_txs();
    cpu_tick_t op_tya();

    template <typename AccessMode> cpu_tick_t op_lax();
    template <typename AccessMode> cpu_tick_t op_sax();
    template <typename AccessMode> cpu_tick_t op_dcp();
    template <typename AccessMode> cpu_tick_t op_isc();
    template <typename AccessMode> cpu_tick_t op_rla();
    template <typename AccessMode> cpu_tick_t op_rra();
    template <typename AccessMode> cpu_tick_t op_slo();
    template <typename AccessMode> cpu_tick_t op_sre();
    template <typename AccessMode> cpu_tick_t op_skb();
    template <typename AccessMode> cpu_tick_t op_ign();
    template <typename AccessMode> cpu_tick_t op_axa();

    cpu_tick_t op_alr();
    cpu_tick_t op_anc();
    cpu_tick_t op_arr();
    cpu_tick_t op_axs();
    cpu_tick_t op_kil();
    cpu_tick_t op_las();
    cpu_tick_t op_sxa();
    cpu_tick_t op_sya();
    cpu_tick_t op_xaa();
    cpu_tick_t op_xas();

private:
    cpu_tick_t branchOnFlag(Registers::Flags flag, bool state);
    uint8_t _add(uint8_t value);
    uint8_t _and(uint8_t a, uint8_t b);
    uint8_t _eor(uint8_t a, uint8_t b);
    uint8_t _or(uint8_t a, uint8_t b);
    uint8_t _lsr(uint8_t value);
    uint8_t _asl(uint8_t value);
    uint8_t _ror(uint8_t value);
    uint8_t _rol(uint8_t value);
    void updateZNFlags(uint8_t value);

private:
    std::shared_ptr<CPUMemory> _memory;
    Registers _registers;
    InstructionHandler _instructions[0x100];
    cpu_tick_t _dmaCycle;
    cpu_cycle _cycle;
    master_cycle _masterClock;
    bool _killed;
};

}

#endif //NESEMU_CPU_H
