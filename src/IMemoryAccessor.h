#ifndef NESCORE_IMEMORYACCESSOR_H
#define NESCORE_IMEMORYACCESSOR_H

#include <cstdint>
#include <string>

namespace nescore
{

class IMemoryAccessor
{
public:
    virtual ~IMemoryAccessor();

    virtual void writeByte(uint16_t offset, uint8_t value) = 0;
    virtual uint8_t readByte(uint16_t offset) = 0;
    virtual uint16_t getStackOffset();

    uint8_t popByte(uint8_t& s);
    uint16_t readShort(uint16_t offset);
    uint16_t popShort(uint8_t& s);
    void readBytes(uint8_t* dst, uint16_t offset, uint16_t size);
    void writeBytes(uint16_t offset, const uint8_t* src, uint16_t size);
    void pushByte(uint8_t& s, uint8_t value);
    void writeShort(uint16_t offset, uint16_t value);
    void pushShort(uint8_t& s, uint16_t value);

};

}


#endif //NESCORE_IMEMORYACCESSOR_H
