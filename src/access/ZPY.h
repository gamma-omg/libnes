#ifndef NESCORE_ACCESSZPY_H
#define NESCORE_ACCESSZPY_H

#include "../CPU.h"
#include "../memory/CPUMemory.h"

namespace nescore
{

class ZPY
{
public:
    ZPY(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = 3;
        _address = readAddress();
        _rw = true;
        return _memory->readByte(_address);
    }

    void write(uint8_t value)
    {
        if (!_rw)
        {
            _address = readAddress();
        }

        _cycles = _rw ? 5 : 3;
        _memory->writeByte(_address, value);
    }

    cpu_cycle_t getCycles() const
    {
        return _cycles;
    }

private:
    uint16_t readAddress()
    {
        uint16_t address = _memory->readByte(_registers.PC++) + _registers.Y;
        address = address & 0xFF;
        return address;
    }

private:
    CPU::Registers& _registers;
    CPUMemory* _memory;
    uint16_t _address;
    cpu_cycle_t _cycles;
    bool _rw;
};

}

#endif //NESCORE_ACCESSZPY_H
