#ifndef NESCORE_NROM_H
#define NESCORE_NROM_H

#include "IRomMapper.h"
#include "../IMemoryAccessor.h"

namespace nescore
{

class NROM : public IRomMapper
{
public:
    class CPUMemory : public IMemoryAccessor
    {
    public:
        CPUMemory(std::shared_ptr<INESRom> rom, std::shared_ptr<Memory> memory);
        CPUMemory(const CPUMemory&) = delete;
        ~CPUMemory();

        void writeByte(uint16_t offset, uint8_t value) override;
        uint8_t readByte(uint16_t offset) override;
        uint16_t getStackOffset() override;

    private:
        std::shared_ptr<INESRom> _rom;
        std::shared_ptr<Memory> _memory;
        uint8_t* _prgRam;
        uint8_t* _prgRom;
    };

    class PPUMemory : public IMemoryAccessor
    {
    public:
        PPUMemory(std::shared_ptr<INESRom> rom);

        uint8_t readByte(uint16_t offset) override;
        void writeByte(uint16_t offset, uint8_t value) override;
    private:
        std::shared_ptr<INESRom> _rom;
    };

public:
    NROM(std::shared_ptr<INESRom> rom, std::shared_ptr<Memory> memory);

    std::shared_ptr<IMemoryAccessor> getCPUMemory() override;
    std::shared_ptr<IMemoryAccessor> getPPUMemory() override;

private:
    std::shared_ptr<CPUMemory> _cpuMemory;
    std::shared_ptr<PPUMemory> _ppuMemory;

};

}

#endif //NESCORE_NROM_H
