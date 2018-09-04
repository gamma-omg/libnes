#include <memory.h>
#include "PPUMemory.h"

namespace nescore
{

const Memory::Range PPUMemory::VRAM = Memory::Range(0x2000, 0x2FFF);
const Memory::Range PPUMemory::VRAM_MIRROR = Memory::Range(0x3000, 0x2EFF);

PPUMemory::PPUMemory()
    : Memory(0x10000)
{
    mirror(VRAM, VRAM_MIRROR);
}

PPUMemory::~PPUMemory()
{
}

}