#include "PPUStatus.h"

namespace nescore
{

PPUStatus::PPUStatus() : _register(0)
{
}

void PPUStatus::setSpriteOverflow(bool value)
{
    auto bit = value ? 1 : 0;
    _register ^= (-bit ^ _register) & Bits::SPRITE_OVERFLOW;
}

void PPUStatus::setSprite0Hit(bool value)
{
    auto bit = value ? 1 : 0;
    _register ^= (-bit ^ _register) & Bits::SPRITE_0_HIT;
}

void PPUStatus::setVBlank(bool value)
{
    auto bit = value ? 1 : 0;
    _register ^= (-bit ^ _register) & Bits::VBLANK;
}

PPUStatus& PPUStatus::operator=(uint8_t value)
{
    _register = value;
    return *this;
}

PPUStatus::operator uint8_t() const
{
    return _register;
}

}