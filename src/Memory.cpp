#include <stdexcept>
#include <memory.h>
#include "Memory.h"

namespace nescore
{

Memory::Memory()
    : _memory(new uint8_t[0xFFFF])
{
    memset(_memory, 0x00, 0xFFFF);
}

Memory::Memory(const std::vector<uint8_t> &ram)
    : Memory()
{
    uint8_t* address = _memory;
    memcpy(address, ram.data(), ram.size()); address += RAM_PAGE_SIZE;
    memcpy(address, ram.data(), ram.size()); address += RAM_PAGE_SIZE;
    memcpy(address, ram.data(), ram.size()); address += RAM_PAGE_SIZE;
    memcpy(address, ram.data(), ram.size());
}

void Memory::loadProgram(const std::vector<uint8_t> &program)
{
    memcpy(_memory + ROM_OFFSET, program.data(), program.size());
    writeResetVecor(ROM_OFFSET);
}

void Memory::writeByte(uint16_t offset, uint8_t value)
{
    if (offset < RAM_SIZE)
    {
        uint16_t address = offset % RAM_PAGE_SIZE;
        _memory[address] = value; address += RAM_PAGE_SIZE;
        _memory[address] = value; address += RAM_PAGE_SIZE;
        _memory[address] = value; address += RAM_PAGE_SIZE;
        _memory[address] = value;
        return;
    }

    _memory[offset] = value;
}

void Memory::writeShort(uint16_t offset, uint16_t value)
{
    writeByte(offset, value & 0xFF);
    writeByte(offset + 1, value >> 8);
}

void Memory::writeBytes(uint16_t offset, const uint8_t *src, uint16_t size)
{
    memcpy(_memory + offset, src, size);
}

uint8_t Memory::readByte(uint16_t offset)
{
    return _memory[offset];
}

uint16_t Memory::readShort(uint16_t offset)
{
    uint8_t l = _memory[offset];
    uint8_t h = _memory[offset + 1];
    return l | (h << 8);
}

void Memory::writeResetVecor(uint16_t address)
{
    writeShort(RESET_VECTOR, address);
}

uint16_t Memory::readResetVector()
{
    return readShort(RESET_VECTOR);
}

std::string Memory::readString(uint16_t offset)
{
    return std::string(reinterpret_cast<char*>(_memory + offset));
}

void Memory::pushByte(uint8_t &s, uint8_t value)
{
    _memory[STACK_BOTTOM + s] = value;
    s--;
}

void Memory::pushShort(uint8_t &s, uint16_t value)
{
    uint8_t l = value & 0xFF;
    uint8_t h = value >> 8;
    pushByte(s, h);
    pushByte(s, l);
}

uint8_t Memory::popByte(uint8_t &s)
{
    s++;
    return _memory[STACK_BOTTOM + s];
}

uint16_t Memory::popShort(uint8_t &s)
{
    uint8_t l = popByte(s);
    uint8_t h = popByte(s);
    return (h << 8) | l;
}

}