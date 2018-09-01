#ifndef NESCORE_PPUMEMORY_H
#define NESCORE_PPUMEMORY_H

#include "../memory/Memory.h"

namespace nescore
{

class PPUMemory : public Memory
{
public:
    static const Memory::Range VRAM;
    static const Memory::Range VRAM_MIRROR;

public:
    PPUMemory();
    ~PPUMemory();

private:
    uint8_t* _vram;
};

}

#endif //NESCORE_PPUMEMORY_H
