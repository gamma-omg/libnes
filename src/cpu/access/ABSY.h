#ifndef NESCORE_ACCESSABSY_H
#define NESCORE_ACCESSABSY_H

#include "../CPU.h"
#include "../../memory/CPUMemory.h"

namespace nescore
{

class ABSY
{
public:
    ABSY(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = 3;
        _address = _memory->readShort(_registers.PC);
        if (_address & 0xFF + _registers.Y > 0xFF)
        {
            _cycles++;
        }

        _address += +_registers.Y;
        _registers.PC += 2;
        _rw = true;
        return _memory->readByte(_address);
    }

    void write(uint8_t value)
    {
        if (!_rw)
        {
            _address = _memory->readShort(_registers.PC) + _registers.Y;
            _registers.PC += 2;
        }

        _cycles = _rw ? 6 : 4;
        _memory->writeByte(_address, value);
    }

    uint16_t getAddress()
    {
        return _memory->readShort(_registers.PC) + _registers.Y;
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

#endif //NESCORE_ACCESSABSY_H
