#ifndef NESCORE_ACCESSACC_H
#define NESCORE_ACCESSACC_H

#include "../CPU.h"
#include "../Memory.h"

namespace nescore
{

class ACC
{
public:
    ACC(CPU::Registers& registers, Memory* memory)
        : _registers(registers)
        , _memory(memory)
    {}

    uint8_t read()
    {
        return _registers.A;
    }

    cpu_cycle_t getCycles() const
    {
        return 1;
    }

private:
    CPU::Registers& _registers;
    Memory* _memory;
};

}

#endif //NESCORE_ACCESSACC_H