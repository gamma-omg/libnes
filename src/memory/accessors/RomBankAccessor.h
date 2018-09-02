#ifndef NESCORE_ROMBANKACCESSOR_H
#define NESCORE_ROMBANKACCESSOR_H

#include "IMemoryAccessor.h"
#include "../../rom/INESRom.h"

namespace nescore
{

class RomBankAccessor : public IMemoryAccessor
{
public:
    RomBankAccessor(const INESRom::Bank* bank);

    void writeByte(uint16_t offset, uint8_t value) override;
    uint8_t readByte(uint16_t offset) const override;

private:
    const INESRom::Bank* _bank;
};

}

#endif //NESCORE_ROMBANKACCESSOR_H
