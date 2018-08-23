#ifndef NESCORE_ACCESSINDY_H
#define NESCORE_ACCESSINDY_H

#include "../CPU.h"
#include "../Memory.h"

namespace nescore
{

class INDY
{
public:
    INDY(CPU::Registers& registers, Memory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = 4;
        _address = _memory->readByte(_registers.PC++);
        _address = _memory->readShort(_address) + _registers.Y;

        _rw = true;
        return _memory->readByte(_address);
    }

    void write(uint8_t value)
    {
        if (!_rw)
        {
            _address = _memory->readByte(_registers.PC++);
            _address = _memory->readShort(_address) + _registers.Y;
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

#endif //NESCORE_ACCESSINDY_H
