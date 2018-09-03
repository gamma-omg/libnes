#ifndef NESCORE_ACCESSZP_H
#define NESCORE_ACCESSZP_H

#include "../CPU.h"
#include "../CPUMemory.h"

namespace nescore
{

class ZP
{
public:
    ZP(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = 2;
        _address = _memory->readByte(_registers.PC++);
        _rw = true;
        return _memory->readByte(_address);
    }

    void write(uint8_t value)
    {
        if (!_rw)
        {
            _address = _memory->readByte(_registers.PC++);
        }

        _cycles = _rw ? 4 : 2;
        _memory->writeByte(_address, value);
    }

    cpu_tick_t getCycles() const
    {
        return _cycles;
    }

private:
    CPU::Registers& _registers;
    CPUMemory* _memory;
    uint16_t _address;
    cpu_tick_t _cycles;
    bool _rw;
};

}

#endif //NESCORE_ACCESSZP_H
