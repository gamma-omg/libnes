#ifndef NESCORE_ACCESSINDY_H
#define NESCORE_ACCESSINDY_H

#include "../CPU.h"
#include "../CPUMemory.h"

namespace nescore
{

class INDY
{
public:
    INDY(CPU::Registers& registers, CPUMemory* memory)
        : _registers(registers)
        , _memory(memory)
        , _cycles(0)
        , _rw(false)
    {}

    uint8_t read()
    {
        _cycles = cpu_cycle(4);
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

        _cycles = _rw ? cpu_cycle(7) : cpu_cycle(5);
        _memory->writeByte(_address, value);
    }

    cpu_cycle getCycles() const
    {
        return _cycles;
    }

private:
    uint16_t getAddress()
    {
        uint8_t base = _memory->readByte(_registers.PC++);
        uint16_t address = 0;
        if (base == 0xFF)
        {
            address = _memory->readByte(0xFF) | _memory->readByte(0x00) << 8;
        }
        else
        {
            address = _memory->readShort(base);
        }

        if (isPageCrossed(address, _registers.Y))
        {
            _cycles++;
        }

        return address + _registers.Y;
    }

    bool isPageCrossed(uint16_t address, uint16_t offset)
    {
        return (address + offset) & 0xFF00 != address & 0xFF00;
    }

private:
    CPU::Registers& _registers;
    CPUMemory* _memory;
    uint16_t _address;
    cpu_cycle _cycles;
    bool _rw;
};

}

#endif //NESCORE_ACCESSINDY_H
