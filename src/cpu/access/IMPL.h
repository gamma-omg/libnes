#ifndef NESCORE_ACCESSIMPL_H
#define NESCORE_ACCESSIMPL_H

#include "../CPU.h"
#include "../CPUMemory.h"

namespace nescore
{

class IMPL
{
public:
    IMPL(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
    {}

    uint8_t read()
    {
        return 0;
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

#endif //NESCORE_ACCESSIMPL_H
