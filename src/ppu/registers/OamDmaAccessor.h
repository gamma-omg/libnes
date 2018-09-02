#ifndef NESCORE_OAMDMAACCESSOR_H
#define NESCORE_OAMDMAACCESSOR_H

#include "../../cpu/CPUMemory.h"

namespace nescore
{

class PPU;

class OamDmaAccessor : public IMemoryAccessor
{
private:
    static const Memory::Range ADDRESS;

public:
    OamDmaAccessor(PPU* ppu);

    void mountTo(std::shared_ptr<CPUMemory> memory);
    void writeByte(uint16_t offset, uint8_t value) override;
    uint8_t readByte(uint16_t offset) const override;

private:
    PPU* _ppu;
};

}

#endif //NESCORE_OAMDMAACCESSOR_H
