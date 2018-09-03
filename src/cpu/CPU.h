#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

#include <vector>
#include <memory>
#include <functional>
#include "../Timings.h"

namespace nescore
{

class CPUMemory;

class CPU
{
private:
    using InstructionHandler = cpu_cycle (CPU::*)();

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

    template <typename AccessMode> cpu_cycle op_adc();
    template <typename AccessMode> cpu_cycle op_and();
    template <typename AccessMode> cpu_cycle op_asl();
    template <typename AccessMode> cpu_cycle op_cmp();
    template <typename AccessMode> cpu_cycle op_cpx();
    template <typename AccessMode> cpu_cycle op_cpy();
    template <typename AccessMode> cpu_cycle op_dec();
    template <typename AccessMode> cpu_cycle op_eor();
    template <typename AccessMode> cpu_cycle op_inc();
    template <typename AccessMode> cpu_cycle op_lda();
    template <typename AccessMode> cpu_cycle op_ldx();
    template <typename AccessMode> cpu_cycle op_ldy();
    template <typename AccessMode> cpu_cycle op_bit();
    template <typename AccessMode> cpu_cycle op_lsr();
    template <typename AccessMode> cpu_cycle op_ora();
    template <typename AccessMode> cpu_cycle op_rol();
    template <typename AccessMode> cpu_cycle op_ror();
    template <typename AccessMode> cpu_cycle op_sbc();
    template <typename AccessMode> cpu_cycle op_sta();
    template <typename AccessMode> cpu_cycle op_stx();
    template <typename AccessMode> cpu_cycle op_sty();

    cpu_cycle op_bcc();
    cpu_cycle op_bcs();
    cpu_cycle op_beq();
    cpu_cycle op_bmi();
    cpu_cycle op_bne();
    cpu_cycle op_bpl();
    cpu_cycle op_brk();
    cpu_cycle op_bvc();
    cpu_cycle op_bvs();
    cpu_cycle op_clc();
    cpu_cycle op_cld();
    cpu_cycle op_cli();
    cpu_cycle op_clv();
    cpu_cycle op_dex();
    cpu_cycle op_dey();
    cpu_cycle op_inx();
    cpu_cycle op_iny();
    cpu_cycle op_jsr();
    cpu_cycle op_jmp_abs();
    cpu_cycle op_jmp_ind();
    cpu_cycle op_nop();
    cpu_cycle op_pha();
    cpu_cycle op_php();
    cpu_cycle op_pla();
    cpu_cycle op_plp();
    cpu_cycle op_rti();
    cpu_cycle op_rts();
    cpu_cycle op_sec();
    cpu_cycle op_sed();
    cpu_cycle op_sei();
    cpu_cycle op_tax();
    cpu_cycle op_tay();
    cpu_cycle op_tsx();
    cpu_cycle op_txa();
    cpu_cycle op_txs();
    cpu_cycle op_tya();

    template <typename AccessMode> cpu_cycle op_lax();
    template <typename AccessMode> cpu_cycle op_sax();
    template <typename AccessMode> cpu_cycle op_dcp();
    template <typename AccessMode> cpu_cycle op_isc();
    template <typename AccessMode> cpu_cycle op_rla();
    template <typename AccessMode> cpu_cycle op_rra();
    template <typename AccessMode> cpu_cycle op_slo();
    template <typename AccessMode> cpu_cycle op_sre();
    template <typename AccessMode> cpu_cycle op_skb();
    template <typename AccessMode> cpu_cycle op_ign();
    template <typename AccessMode> cpu_cycle op_axa();

    cpu_cycle op_alr();
    cpu_cycle op_anc();
    cpu_cycle op_arr();
    cpu_cycle op_axs();
    cpu_cycle op_kil();
    cpu_cycle op_las();
    cpu_cycle op_sxa();
    cpu_cycle op_sya();
    cpu_cycle op_xaa();
    cpu_cycle op_xas();

private:
    cpu_cycle branchOnFlag(Registers::Flags flag, bool state);
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
    cpu_cycle _cycle;
    cpu_cycle _dmaCycle;
    master_cycle _masterClock;
    bool _killed;
};

}

#endif //NESEMU_CPU_H
