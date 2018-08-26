#ifndef NESEMU_MEMORY_H
#define NESEMU_MEMORY_H

#include <cstdint>
#include <vector>
#include <memory>
#include "IMemoryAccessor.h"

namespace nescore
{

class PPU;

class Memory : public IMemoryAccessor
{
public:
    static const uint16_t RAM_PAGE_SIZE = 0x0800;
    static const uint16_t RAM_SIZE = 0x2000;
    static const uint16_t PRG_RAM_OFFSET = 0x6000;
    static const uint16_t ROM_OFFSET = 0x8000;
    static const uint16_t ROM_SIZE = 0x7FFF;
    static const uint16_t RESET_VECTOR = 0xFFFC;
    static const uint16_t IRQ_VECTOR = 0xFFFE;
    static const uint16_t NMI_VECTOR = 0xFFFA;
    static const uint16_t STACK_TOP = 0x01FF;
    static const uint16_t STACK_BOTTOM = 0x0100;

public:
    Memory();
    Memory(std::shared_ptr<PPU> ppu);
    Memory(const std::vector<uint8_t>& ram);
    ~Memory();

    uint8_t readByte(uint16_t offset) override;
    uint8_t popByte(uint8_t& s) override;
    void readBytes(uint8_t *dst, uint16_t offset, uint16_t size) override;
    void writeByte(uint16_t offset, uint8_t value) override;
    void writeBytes(uint16_t offset, const uint8_t* src, uint16_t size) override;
    void pushByte(uint8_t& s, uint8_t value) override;

    void loadProgram(const std::vector<uint8_t> &program);
    std::string readString(uint16_t offset);

private:
    uint8_t* _memory;
    std::shared_ptr<PPU> _ppu;
};

}


#endif //NESEMU_MEMORY_H
