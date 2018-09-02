#ifndef NESCORE_MIRRORACCESSOR_H
#define NESCORE_MIRRORACCESSOR_H

#include "IMemoryAccessor.h"
#include "../Memory.h"

namespace nescore
{

class MirrorAccessor : public IMemoryAccessor
{
public:
    MirrorAccessor(IMemoryAccessor* source, Memory::Range range);

    void writeByte(uint16_t offset, uint8_t value) override;
    uint8_t readByte(uint16_t offset) const override;

private:
    IMemoryAccessor* _source;
    Memory::Range _range;

};

}

#endif //NESCORE_MIRRORACCESSOR_H
