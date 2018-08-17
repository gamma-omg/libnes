#ifndef NESCORE_ACCESSIMPL_H
#define NESCORE_ACCESSIMPL_H

#include "../CPU.h"
#include "../Memory.h"

namespace nescore
{

class IMPL
{
public:
    IMPL(CPU::Registers& registers, Memory* memory)
        : _registers(registers)
        , _memory(memory)
    {}

    uint8_t read()
    {
        return 0;
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

#endif //NESCORE_ACCESSIMPL_H
