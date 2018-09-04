#include "CPUMemory.h"

namespace nescore
{

const Memory::Range CPUMemory::RAM = Range(0x0000, 0xFFFF);
const Memory::Range CPUMemory::RAM_MIRROR_1 = Range(0x0800, 0x0FFF);
const Memory::Range CPUMemory::RAM_MIRROR_2 = Range(0x1000, 0x17FF);
const Memory::Range CPUMemory::RAM_MIRROR_3 = Range(0x1800, 0x1FFF);

CPUMemory::CPUMemory()
    : Memory(0x10000)
{
    mirror(RAM, RAM_MIRROR_1);
    mirror(RAM, RAM_MIRROR_2);
    mirror(RAM, RAM_MIRROR_3);

    setStackOffset(STACK_OFFSET);
}

CPUMemory::CPUMemory(const std::vector<uint8_t>& ram)
    : CPUMemory()
{
    writeBytes(ram.data(), 0, ram.size());
}

CPUMemory::~CPUMemory()
{
}

void CPUMemory::loadProgram(const std::vector<uint8_t> &program)
{
    writeBytes(program.data(), ROM_OFFSET, program.size());
}

void CPUMemory::setResetVector(uint16_t offset)
{
    writeShort(RESET_VECTOR, offset);
}

}