#include "IMemoryAccessor.h"

namespace nescore
{

IMemoryAccessor::~IMemoryAccessor()
{
}

uint16_t IMemoryAccessor::readShort(uint16_t offset)
{
    uint8_t l = readByte(offset);
    uint8_t h = readByte(offset + 1);
    return l | (h << 8);
}

void IMemoryAccessor::writeShort(uint16_t offset, uint16_t value)
{
    writeByte(offset, value & 0xFF);
    writeByte(offset + 1, value >> 8);
}

uint16_t IMemoryAccessor::popShort(uint8_t &s)
{
    uint8_t l = popByte(s);
    uint8_t h = popByte(s);
    return (h << 8) | l;
}

void IMemoryAccessor::pushShort(uint8_t &s, uint16_t value)
{
    uint8_t l = value & 0xFF;
    uint8_t h = value >> 8;
    pushByte(s, h);
    pushByte(s, l);
}

uint8_t IMemoryAccessor::popByte(uint8_t &s)
{
    s++;
    return readByte(getStackOffset() + s);
}

void IMemoryAccessor::readBytes(uint8_t* dst, uint16_t offset, uint16_t size)
{
    for (auto i = 0; i < size; ++i)
    {
        *dst = readByte(offset + i);
        dst++;
    }
}

void IMemoryAccessor::writeBytes(uint16_t offset, const uint8_t *src, uint16_t size)
{
    for (auto i = 0; i < size; ++i)
    {
        writeByte(offset + i, *src);
        src++;
    }
}

void IMemoryAccessor::pushByte(uint8_t &s, uint8_t value)
{
    writeByte(getStackOffset() + s, value);
    s--;
}

uint16_t IMemoryAccessor::getStackOffset()
{
    return 0;
}

}