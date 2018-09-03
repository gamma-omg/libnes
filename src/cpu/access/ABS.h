#ifndef NESCORE_ACCESSABS_H
#define NESCORE_ACCESSABS_H

#include "../CPU.h"
#include "../CPUMemory.h"

namespace nescore
{

class ABS
{
public:
    ABS(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = cpu_cycle(3);
        _address = _memory->readShort(_registers.PC);
        _registers.PC += 2;
        _rw = true;
        return _memory->readByte(_address);
    }

    void write(uint8_t value)
    {
        if (!_rw)
        {
            _address = _memory->readShort(_registers.PC);
            _registers.PC += 2;
        }

        _cycles = _rw ? cpu_cycle(5) : cpu_cycle(3);
        _memory->writeByte(_address, value);
    }

    cpu_cycle getCycles() const
    {
        return _cycles;
    }

private:
    CPU::Registers& _registers;
    CPUMemory* _memory;
    uint16_t _address;
    cpu_cycle _cycles;
    bool _rw;
};

}

#endif //NESCORE_ACCESSABS_H
