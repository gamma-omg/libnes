#include <memory.h>
#include "PPUMemory.h"

namespace nescore
{

const Memory::Range PPUMemory::VRAM = Memory::Range(0x2000, 0x2FFF);
const Memory::Range PPUMemory::VRAM_MIRROR = Memory::Range(0x3000, 0x2EFF);

PPUMemory::PPUMemory()
    : _vram(new uint8_t[0x2000])
{
    mount(VRAM, _vram);
    mirror(VRAM, VRAM_MIRROR);

    memset(_vram, 0x00, sizeof(uint8_t) * 0x2000);
}

PPUMemory::~PPUMemory()
{
    delete[] _vram;
}

}