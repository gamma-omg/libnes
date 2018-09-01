#ifndef NESCORE_ACCESSABSX_H
#define NESCORE_ACCESSABSX_H

#include "../CPU.h"
#include "../CPUMemory.h"

namespace nescore
{

class ABSX
{
public:
    ABSX(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = 3;
        _address = _memory->readShort(_registers.PC);
        if (_address & 0xFF + _registers.X > 0xFF)
        {
            _cycles++;
        }

        _address += +_registers.X;
        _registers.PC += 2;
        _rw = true;
        return _memory->readByte(_address);
    }

    void write(uint8_t value)
    {
        if (!_rw)
        {
            _address = _memory->readShort(_registers.PC) + _registers.X;
            _registers.PC += 2;
        }

        _cycles = _rw ? 6 : 4;
        _memory->writeByte(_address, value);
    }

    uint16_t getAddress()
    {
        return _memory->readShort(_registers.PC) + _registers.X;
    }

    cpu_cycle_t getCycles() const
    {
        return _cycles;
    }

private:
    CPU::Registers& _registers;
    CPUMemory* _memory;
    uint16_t _address;
    cpu_cycle_t _cycles;
    bool _rw;
};

}

#endif //NESCORE_ACCESSABSX_H
