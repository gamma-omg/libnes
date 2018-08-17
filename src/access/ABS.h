#ifndef NESCORE_ACCESSABS_H
#define NESCORE_ACCESSABS_H

#include "../CPU.h"
#include "../Memory.h"

namespace nescore
{

class ABS
{
public:
    ABS(CPU::Registers& registers, Memory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles= 3;
        _address = _memory->readShort(_registers.PC);
        _registers.PC += 2;
        _rw = true;
        return _memory->readByte(_address);
    }

    void write(uint8_t value)
    {
        if (!_rw)
        {
            _address = _memory->readByte(_registers.PC++);
        }

        _cycles = _rw ? 5 : 3;
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

#endif //NESCORE_ACCESSABS_H
