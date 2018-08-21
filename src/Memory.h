#ifndef NESEMU_MEMORY_H
#define NESEMU_MEMORY_H

#include <cstdint>
#include <vector>

namespace nescore
{

class Memory
{
public:
    static const uint16_t RAM_PAGE_SIZE = 0x0800;
    static const uint16_t RAM_SIZE = 0x2000;
    static const uint16_t PRG_RAM_OFFSET = 0x6000;
    static const uint16_t ROM_OFFSET = 0x8000;
    static const uint16_t ROM_SIZE = 0x7FFF;
    static const uint16_t RESET_VECTOR = 0xFFFC;
    static const uint16_t STACK_TOP = 0x01FF;
    static const uint16_t STACK_BOTTOM = 0x0100;

public:
    Memory();
    Memory(const std::vector<uint8_t>& ram);

    void loadROM(const std::vector<uint8_t>& rom);
    void writeByte(uint16_t offset, uint8_t value);
    void writeShort(uint16_t offset, uint16_t value);
    void writeBrkVecor(uint16_t address);
    void writeByteToStack(uint8_t top, uint8_t value);
    void writeShortToStack(uint8_t top, uint16_t value);
    void writeBytes(uint16_t offset, const uint8_t* src, uint16_t size);
    uint8_t readByte(uint16_t offset);
    uint16_t readShort(uint16_t offset);
    uint16_t readBrkVector();
    uint8_t readByteFromStack(uint8_t top);
    uint16_t readShortFromStack(uint8_t top);
    std::string readString(uint16_t offset);

private:
    uint8_t* _memory;
};

}


#endif //NESEMU_MEMORY_H
