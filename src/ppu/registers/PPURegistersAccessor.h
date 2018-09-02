#ifndef NESCORE_PPUREGISTERSACCESSOR_H
#define NESCORE_PPUREGISTERSACCESSOR_H

#include <memory>
#include "../../cpu/CPUMemory.h"

namespace nescore
{

class PPU;

class PPURegistersAccessor : public IMemoryAccessor
{
private:
    static const Memory::Range RANGE;
    static const Memory::Range MIRROR;

    static const uint16_t PPUCTRL = 0x0;
    static const uint16_t PPUMASK = 0x1;
    static const uint16_t PPUSTATUS = 0x2;
    static const uint16_t OAMADDR = 0x3;
    static const uint16_t OAMDATA = 0x4;
    static const uint16_t PPUSCROLL = 0x5;
    static const uint16_t PPUADDR = 0x6;
    static const uint16_t PPUDATA = 0x7;


public:
    PPURegistersAccessor(PPU* ppu);

    void mountTo(std::shared_ptr<CPUMemory> memory);
    void writeByte(uint16_t offset, uint8_t value) override;
    uint8_t readByte(uint16_t offset) const override;

private:
    PPU* _ppu;
};

}

#endif //NESCORE_PPUREGISTERSACCESSOR_H
