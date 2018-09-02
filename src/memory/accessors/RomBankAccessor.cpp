#include "RomBankAccessor.h"

namespace nescore
{

RomBankAccessor::RomBankAccessor(const INESRom::Bank* bank)
    : _bank(bank)
{
}

void RomBankAccessor::writeByte(uint16_t offset, uint8_t value)
{
}

uint8_t RomBankAccessor::readByte(uint16_t offset) const
{
    return _bank->readByte(offset % _bank->getSize());
}

}
