#ifndef NESCORE_ACCESSZPX_H
#define NESCORE_ACCESSZPX_H

#include "../CPU.h"
#include "../CPUMemory.h"

namespace nescore
{

class ZPX
{
public:
    ZPX(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = cpu_cycle(3);
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

        _cycles = _rw ? cpu_cycle(5) : cpu_cycle(3);
        _memory->writeByte(_address, value);
    }

    cpu_cycle getCycles() const
    {
        return _cycles;
    }

private:
    uint16_t readAddress()
    {
        uint16_t address = _memory->readByte(_registers.PC++) + _registers.X;
        address = address & 0xFF;
        return address;
    }

private:
    CPU::Registers& _registers;
    CPUMemory* _memory;
    uint16_t _address;
    cpu_cycle _cycles;
    bool _rw;
};

}

#endif //NESCORE_ACCESSZPX_H
