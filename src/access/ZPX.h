#ifndef NESCORE_ACCESSZPX_H
#define NESCORE_ACCESSZPX_H

#include "../CPU.h"
#include "../Memory.h"

namespace nescore
{

class ZPX
{
public:
    ZPX(CPU::Registers& registers, Memory* memory)
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
        auto address = _memory->readByte(_registers.PC++);
        address += _registers.X;
        address %= 0xFF;
        return address;
    }

private:
    CPU::Registers& _registers;
    Memory* _memory;
    uint16_t _address;
    cpu_cycle_t _cycles;
    bool _rw;
};

}

#endif //NESCORE_ACCESSZPX_H