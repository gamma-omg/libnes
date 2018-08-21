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

void Memory::loadROM(const std::vector<uint8_t> &rom)
{
    memcpy(_memory + ROM_OFFSET, rom.data(), rom.size());
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

void Memory::writeBrkVecor(uint16_t address)
{
    writeShort(RESET_VECTOR, address);
}

uint16_t Memory::readBrkVector()
{
    return readShort(RESET_VECTOR);
}

uint8_t Memory::readByteFromStack(uint8_t top)
{
    return readByte(STACK_BOTTOM + top);
}

uint16_t Memory::readShortFromStack(uint8_t top)
{
    return readShort(STACK_BOTTOM + top);
}

std::string Memory::readString(uint16_t offset)
{
    return std::string(reinterpret_cast<char*>(_memory + offset));
}

}