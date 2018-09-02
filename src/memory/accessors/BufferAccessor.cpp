#include "BufferAccessor.h"

namespace nescore
{

void BufferAccessor::setBuffer(uint8_t *buffer)
{
    _buffer = buffer;
}

void BufferAccessor::writeByte(uint16_t offset, uint8_t value)
{
    _buffer[offset] = value;
}

uint8_t BufferAccessor::readByte(uint16_t offset) const
{
    return _buffer[offset];
}


}
