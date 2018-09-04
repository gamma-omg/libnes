#include "PPUAddress.h"

namespace nescore
{

PPUAddress::PPUAddress() : _register(0), _writeMask(0xFF00)
{
}

PPUAddress& PPUAddress::operator=(uint8_t value)
{
    _register = value & _writeMask;
    _writeMask = _writeMask > 0xFF ? _writeMask >> 8 : 0xFF00;
    return *this;
}

PPUAddress &PPUAddress::operator+=(uint16_t value)
{
    _register += value;
    return *this;
}

PPUAddress::operator uint16_t() const
{
    return _register;
}

}
