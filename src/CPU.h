#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

#include <vector>
#include <memory>
#include <functional>

namespace nescore
{

class Memory;

class CPU
{
private:
    using InstructionHandler = std::function<void()>;
    using AccessMethod = uint8_t (CPU::*)();
    using AddressReader = uint16_t (CPU::*)();

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

    void tick();
    void tick(int count);
    const Registers& getRegisters() const;
    Registers& getRegisters();
    std::shared_ptr<Memory> getMemory();

private:
    void initHandlers();

    void op_adc(AccessMethod accessMethod);
    void op_and(AccessMethod accessMethod);
    void op_asl(AccessMethod accessMethod);
    void op_bcc(AccessMethod accessMethod);
    void op_bcs(AccessMethod accessMethod);
    void op_beq(AccessMethod accessMethod);
    void op_bit(AccessMethod accessMethod);
    void op_bmi(AccessMethod accessMethod);
    void op_bne(AccessMethod accessMethod);
    void op_bpl(AccessMethod accessMethod);
    void op_brk(AccessMethod accessMethod);
    void op_bvc(AccessMethod accessMethod);
    void op_bvs(AccessMethod accessMethod);
    void op_clc(AccessMethod accessMethod);
    void op_cld(AccessMethod accessMethod);
    void op_cli(AccessMethod accessMethod);
    void op_clv(AccessMethod accessMethod);
    void op_cmp(AccessMethod accessMethod);
    void op_cpx(AccessMethod accessMethod);
    void op_cpy(AccessMethod accessMethod);
    void op_dec(AddressReader addressReader);
    void op_dex(AccessMethod accessMethod);
    void op_dey(AccessMethod accessMethod);
    void op_eor(AccessMethod accessMethod);
    void op_inc(AddressReader addressReader);
    void op_inx(AccessMethod accessMethod);
    void op_iny(AccessMethod accessMethod);
    void op_jmp(AddressReader addressReader);
    void op_jsr(AddressReader addressReader);
    void op_lda(AccessMethod accessMethod);
    void op_ldx(AccessMethod accessMethod);
    void op_ldy(AccessMethod accessMethod);

    uint8_t access_acc();
    uint8_t access_imm();
    uint8_t access_zp();
    uint8_t access_zpx();
    uint8_t access_zpy();
    uint8_t access_abs();
    uint8_t access_absx();
    uint8_t access_absy();
    uint8_t access_indx();
    uint8_t access_indy();
    uint8_t access_rel();
    uint8_t access_impl();

    uint16_t address_zp();
    uint16_t address_zpx();
    uint16_t address_abs();
    uint16_t address_absx();
    uint16_t address_ind();

private:
    void conditionalJump(Registers::Flags flag, bool state, AccessMethod accessMethod);

private:
    std::shared_ptr<Memory> _memory;
    Registers _registers;
    InstructionHandler _commandHandlers[0xFF];

};

}

#endif //NESEMU_CPU_H
