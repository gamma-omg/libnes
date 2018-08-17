#ifndef NESCORE_ACCESSIMM_H
#define NESCORE_ACCESSIMM_H

#include "../CPU.h"
#include "../Memory.h"

namespace nescore
{

class IMM
{
public:
    IMM(CPU::Registers& registers, Memory* memory)
        : _registers(registers)
        , _memory(memory)
    {}

    uint8_t read()
    {
        return _memory->readByte(_registers.PC++);
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

#endif //NESCORE_ACCESSIMM_H