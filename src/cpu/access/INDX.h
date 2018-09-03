#ifndef NESCORE_ACCESSINDX_H
#define NESCORE_ACCESSINDX_H

#include "../CPU.h"
#include "../CPUMemory.h"

namespace nescore
{

class INDX
{
public:
    INDX(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = 4;
        _address = getAddress();
        _rw = true;

        return _memory->readByte(_address);
    }

    void write(uint8_t value)
    {
        if (!_rw)
        {
            _address = getAddress();
        }

        _cycles = _rw ? 7 : 5;
        _memory->writeByte(_address, value);
    }

    cpu_tick_t getCycles() const
    {
        return _cycles;
    }

private:
    uint16_t getAddress()
    {
        uint8_t address = _memory->readByte(_registers.PC++) + _registers.X;
        if (address == 0xFF)
        {
            return _memory->readByte(0xFF) | _memory->readByte(0x00) << 8;
        }

        return _memory->readShort(address);
    }

private:
    CPU::Registers& _registers;
    CPUMemory* _memory;
    uint16_t _address;
    cpu_tick_t _cycles;
    bool _rw;
};

}

#endif //NESCORE_ACCESSINDX_H
