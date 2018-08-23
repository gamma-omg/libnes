#ifndef NESCORE_ACCESSINDX_H
#define NESCORE_ACCESSINDX_H

#include "../CPU.h"
#include "../Memory.h"

namespace nescore
{

class INDX
{
public:
    INDX(CPU::Registers& registers, Memory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = 4;
        _address = _memory->readByte(_registers.PC++) + _registers.X;
        if (_address > 0xFF)
        {
            _cycles++;
            _address &= 0xFF;
        }

        _address = _memory->readByte(_address);

        _rw = true;
        return _memory->readByte(_address);
    }

    void write(uint8_t value)
    {
        if (!_rw)
        {
            _address = (_memory->readByte(_registers.PC++) + _registers.X) & 0xFF;
            _address = _memory->readByte(_address);
        }

        _cycles = _rw ? 7 : 5;
        _memory->writeByte(_address, value);
    }

    cpu_cycle_t getCycles() const
    {
        return _cycles;
    }

private:
    CPU::Registers& _registers;
    Memory* _memory;
    uint16_t _address;
    cpu_cycle_t _cycles;
    bool _rw;
};

}

#endif //NESCORE_ACCESSINDX_H
