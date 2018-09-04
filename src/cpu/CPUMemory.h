#ifndef NESCORE_CPUMEMORY_H
#define NESCORE_CPUMEMORY_H

#include <vector>
#include "../memory/Memory.h"

namespace nescore
{

class CPUMemory : public Memory
{
public:
    static const Range RAM;
    static const Range RAM_MIRROR_1;
    static const Range RAM_MIRROR_2;
    static const Range RAM_MIRROR_3;

    static const uint16_t ROM_OFFSET = 0x8000;
    static const uint16_t RESET_VECTOR = 0xFFFC;
    static const uint16_t IRQ_VECTOR = 0xFFFE;
    static const uint16_t NMI_VECTOR = 0xFFFA;
    static const uint16_t STACK_OFFSET = 0x0100;

public:
    CPUMemory();
    CPUMemory(const std::vector<uint8_t>& ram);
    ~CPUMemory();

    void loadProgram(const std::vector<uint8_t>& program);
    void setResetVector(uint16_t offset);
};

}

#endif //NESCORE_CPUMEMORY_H
