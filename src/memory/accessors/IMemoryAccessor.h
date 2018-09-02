#ifndef NESCORE_IMEMORYACCESSOR_H
#define NESCORE_IMEMORYACCESSOR_H

#include <cstdint>
#include <string>

namespace nescore
{

class IMemoryAccessor
{
public:
    virtual ~IMemoryAccessor() {};

    virtual void writeByte(uint16_t offset, uint8_t value) = 0;
    virtual uint8_t readByte(uint16_t offset) const = 0;
};

}


#endif //NESCORE_IMEMORYACCESSOR_H
