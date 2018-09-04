#ifndef NESCORE_PPUADDRESS_H
#define NESCORE_PPUADDRESS_H

#include <cstdint>

namespace nescore
{

class PPUAddress
{
public:
    PPUAddress();

    PPUAddress& operator=(uint8_t value);
    PPUAddress& operator+=(uint16_t value);
    operator uint16_t() const;

private:
    uint16_t _register;
    uint16_t _writeMask;
};

}

#endif //NESCORE_PPUADDRESS_H
