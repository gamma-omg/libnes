#ifndef NESCORE_BUFFERACCESSOR_H
#define NESCORE_BUFFERACCESSOR_H

#include "IMemoryAccessor.h"

namespace nescore
{

class BufferAccessor : public IMemoryAccessor
{
public:
    void writeByte(uint16_t offset, uint8_t value) override;
    uint8_t readByte(uint16_t offset) override;

    void setBuffer(uint8_t* buffer);

private:
    uint8_t* _buffer;
};

}

#endif //NESCORE_BUFFERACCESSOR_H
