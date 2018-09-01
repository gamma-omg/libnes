#ifndef NESCORE_PPUSTATUS_H
#define NESCORE_PPUSTATUS_H

#include <cstdint>

namespace nescore
{

class PPUStatus
{
private:
    enum Bits
    {
        SPRITE_OVERFLOW = 0x00100000,
        SPRITE_0_HIT = 0x01000000,
        VBLANK = 0x10000000
    };

public:
    PPUStatus();

    void setSpriteOverflow(bool value);
    void setSprite0Hit(bool value);
    void setVBlank(bool value);

    PPUStatus& operator=(uint8_t value);
    operator uint8_t() const;

private:
    uint8_t _register;
};

}

#endif //NESCORE_PPUSTATUS_H
