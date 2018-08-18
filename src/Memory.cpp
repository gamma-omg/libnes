#include <stdexcept>
#include "Memory.h"

namespace nescore
{

Memory::Memory()
{
    _ram.resize(2048);
}

Memory::Memory(const std::vector<uint8_t> &ram)
    : _ram(ram)
{
    _ram.resize(2048);
}

void Memory::loadROM(const std::vector<uint8_t> &rom)
{
    _rom = rom;
    _rom.resize(ROM_SIZE);
}

void Memory::writeByte(uint16_t offset, uint8_t value)
{
    if (offset >= RAM_SIZE)
    {
        throw std::runtime_error("Invalud RAM address: " + offset);
    }

    _ram[offset % RAM_PAGE_SIZE] = value;
}

void Memory::writeShort(uint16_t offset, uint16_t value)
{
    writeByte(offset, value & 0xFF);
    writeByte(offset + 1, value >> 8);
}

void Memory::writeByteToStack(uint8_t top, uint8_t value)
{
    writeByte(STACK_BOTTOM + top, value);
}

void Memory::writeShortToStack(uint8_t top, uint16_t value)
{
    writeShort(STACK_BOTTOM + top, value);
}

void Memory::writeBytes(uint16_t offset, const uint8_t *src, uint16_t size)
{
}

uint8_t Memory::readByte(uint16_t offset)
{
    if (offset < RAM_SIZE)
    {
        return _ram[offset % RAM_PAGE_SIZE];
    }
    if (offset >= ROM_OFFSET)
    {
        return _rom[offset - ROM_OFFSET];
    }

    return 0;
}

uint16_t Memory::readShort(uint16_t offset)
{
    uint8_t l = readByte(offset);
    uint8_t h = readByte(offset + 1);
    return l | (h << 8);
}

void Memory::writeBrkVecor(uint16_t address)
{
    _rom[VECTOR_BRK - ROM_OFFSET] = address & 0xFF;
    _rom[VECTOR_BRK - ROM_OFFSET + 1] = address >> 8;
}

uint16_t Memory::readBrkVector()
{
    uint8_t l = _rom[VECTOR_BRK - ROM_OFFSET];
    uint8_t h = _rom[VECTOR_BRK - ROM_OFFSET + 1];
    return l | (h << 8);
}

uint8_t Memory::readByteFromStack(uint8_t top)
{
    return readByte(STACK_BOTTOM + top);
}

uint16_t Memory::readShortFromStack(uint8_t top)
{
    return readShort(STACK_BOTTOM + top);
}

}