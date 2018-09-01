#include "PPUScroll.h"

namespace nescore
{

PPUScroll::PPUScroll() : _component(0)
{
    _scrolls[0] = 0;
    _scrolls[1] = 0;
}

uint8_t PPUScroll::getX() const
{
    return _scrolls[0];
}

uint8_t PPUScroll::getY() const
{
    return _scrolls[1];
}

PPUScroll& PPUScroll::operator=(uint8_t value)
{
    _scrolls[_component] = value;
    _component = (_component + 1) % 2;
    return *this;
}

}
