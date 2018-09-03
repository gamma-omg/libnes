#ifndef NESCORE_ACCESSACC_H
#define NESCORE_ACCESSACC_H

#include "../CPU.h"
#include "../CPUMemory.h"

namespace nescore
{

class ACC
{
public:
    ACC(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
    {}

    uint8_t read()
    {
        return _registers.A;
    }

    void write(uint8_t value)
    {
        _registers.A = value;
    }

    cpu_tick_t getCycles() const
    {
        return 1;
    }

private:
    CPU::Registers& _registers;
    CPUMemory* _memory;
};

}

#endif //NESCORE_ACCESSACC_H
