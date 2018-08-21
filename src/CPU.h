#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

#include <vector>
#include <memory>
#include <functional>

namespace nescore
{

typedef unsigned int cpu_cycle_t;

class Memory;

class CPU
{
private:
    using InstructionHandler = cpu_cycle_t (CPU::*)();

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
    CPU(std::shared_ptr<Memory> memory);
    CPU(const std::vector<uint8_t>& operations);

    void reset();
    void tick();
    void tick(int count);
    const Registers& getRegisters() const;
    Registers& getRegisters();
    std::shared_ptr<Memory> getMemory();
    cpu_cycle_t getCycle() const;

private:
    void setupInstructions();

    template <typename AccessMode> cpu_cycle_t op_adc();
    template <typename AccessMode> cpu_cycle_t op_and();
    template <typename AccessMode> cpu_cycle_t op_asl();
    template <typename AccessMode> cpu_cycle_t op_cmp();
    template <typename AccessMode> cpu_cycle_t op_cpx();
    template <typename AccessMode> cpu_cycle_t op_cpy();
    template <typename AccessMode> cpu_cycle_t op_dec();
    template <typename AccessMode> cpu_cycle_t op_eor();
    template <typename AccessMode> cpu_cycle_t op_inc();
    template <typename AccessMode> cpu_cycle_t op_lda();
    template <typename AccessMode> cpu_cycle_t op_ldx();
    template <typename AccessMode> cpu_cycle_t op_ldy();
    template <typename AccessMode> cpu_cycle_t op_bit();
    template <typename AccessMode> cpu_cycle_t op_lsr();
    template <typename AccessMode> cpu_cycle_t op_ora();
    template <typename AccessMode> cpu_cycle_t op_rol();
    template <typename AccessMode> cpu_cycle_t op_ror();
    template <typename AccessMode> cpu_cycle_t op_sbc();
    template <typename AccessMode> cpu_cycle_t op_sta();
    template <typename AccessMode> cpu_cycle_t op_stx();
    template <typename AccessMode> cpu_cycle_t op_sty();

    cpu_cycle_t op_bcc();
    cpu_cycle_t op_bcs();
    cpu_cycle_t op_beq();
    cpu_cycle_t op_bmi();
    cpu_cycle_t op_bne();
    cpu_cycle_t op_bpl();
    cpu_cycle_t op_brk();
    cpu_cycle_t op_bvc();
    cpu_cycle_t op_bvs();
    cpu_cycle_t op_clc();
    cpu_cycle_t op_cld();
    cpu_cycle_t op_cli();
    cpu_cycle_t op_clv();
    cpu_cycle_t op_dex();
    cpu_cycle_t op_dey();
    cpu_cycle_t op_inx();
    cpu_cycle_t op_iny();
    cpu_cycle_t op_jsr();
    cpu_cycle_t op_jmp_abs();
    cpu_cycle_t op_jmp_ind();
    cpu_cycle_t op_nop();
    cpu_cycle_t op_pha();
    cpu_cycle_t op_php();
    cpu_cycle_t op_pla();
    cpu_cycle_t op_plp();
    cpu_cycle_t op_rti();
    cpu_cycle_t op_rts();
    cpu_cycle_t op_sec();
    cpu_cycle_t op_sed();
    cpu_cycle_t op_sei();
    cpu_cycle_t op_tax();
    cpu_cycle_t op_tay();
    cpu_cycle_t op_tsx();
    cpu_cycle_t op_txa();
    cpu_cycle_t op_txs();
    cpu_cycle_t op_tya();

private:
    cpu_cycle_t branchOnFlag(Registers::Flags flag, bool state);
    void addToA(uint8_t value);
    void updateZNFlags(uint8_t value);

private:
    std::shared_ptr<Memory> _memory;
    Registers _registers;
    InstructionHandler _instructions[0xFF];
    cpu_cycle_t _cycle;
};

}

#endif //NESEMU_CPU_H
