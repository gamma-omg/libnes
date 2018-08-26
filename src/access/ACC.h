#ifndef NESCORE_ACCESSACC_H
#define NESCORE_ACCESSACC_H

#include "../CPU.h"
#include "../IMemoryAccessor.h"

namespace nescore
{

class ACC
{
public:
    ACC(CPU::Registers& registers, IMemoryAccessor* memory)
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

    cpu_cycle_t getCycles() const
    {
        return 1;
    }

private:
    CPU::Registers& _registers;
    IMemoryAccessor* _memory;
};

}

#endif //NESCORE_ACCESSACC_H
