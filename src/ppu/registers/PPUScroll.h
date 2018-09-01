#ifndef NESCORE_PPUSCROLL_H
#define NESCORE_PPUSCROLL_H

#include <cstdint>

namespace nescore
{

class PPUScroll
{
public:
    PPUScroll();

    uint8_t getX() const;
    uint8_t getY() const;

    PPUScroll& operator=(uint8_t value);

private:
    uint8_t _scrolls[2];
    uint8_t _component;
};

}

#endif //NESCORE_PPUSCROLL_H
