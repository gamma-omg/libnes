#ifndef NESCORE_MEMORY_H
#define NESCORE_MEMORY_H

#include <cstdint>
#include <string>
#include <list>
#include <memory>
#include <stdexcept>
#include "accessors/IMemoryAccessor.h"
#include "../rom/INESRom.h"

namespace nescore
{

class nes_memory_error : std::runtime_error
{
public:
    nes_memory_error(const std::string& message) : std::runtime_error(message)
    {
    }
};

class Memory : public IMemoryAccessor
{
public:
    enum MountMode
    {
        Read = 0b01,
        Write = 0b10,
        ReadWrite = 0b11
    };

    struct Range
    {
        static Range fromBank(uint8_t bank, uint16_t bankSize);

        uint16_t start;
        uint16_t end;

        Range(uint16_t start, uint16_t end);

        bool contains(uint16_t offset);
        uint16_t getGlobalOffset(uint16_t offset);
    };

    struct Mount
    {
        Range range;
        IMemoryAccessor* accessor;

        Mount(Range, IMemoryAccessor* accessor);
    };

public:
    Memory();
    virtual ~Memory();

    uint8_t readByte(uint16_t offset) override;
    void writeByte(uint16_t offset, uint8_t value) override;

    uint8_t popByte(uint8_t& s);
    uint16_t readShort(uint16_t offset);
    uint16_t popShort(uint8_t& s);

    void writeShort(uint16_t offset, uint16_t value);
    void pushByte(uint8_t& s, uint8_t value);
    void pushShort(uint8_t& s, uint16_t value);
    void readBytes(IMemoryAccessor* dst, uint16_t offset, uint16_t size);
    void readBytes(uint8_t* dst, uint16_t offset, uint16_t size);
    void writeBytes(IMemoryAccessor* src, uint16_t offset, uint16_t size);
    void setStackOffset(uint16_t offset);

    std::string readString(uint16_t offset);

    void mount(Range range, IMemoryAccessor* accessor, MountMode mode = MountMode::ReadWrite);
    void mount(Range range, uint8_t* buffer, MountMode mode = MountMode::ReadWrite);
    void mount(Range range, const INESRom::Bank& bank, MountMode mode = MountMode::ReadWrite);
    void mirror(Range src, Range dst, MountMode mode = MountMode::ReadWrite);
    void unmountAll();

private:
    Mount* findMount(std::list<Mount>& source, uint16_t offset);

private:
    std::list<Mount> _readMounts;
    std::list<Mount> _writeMounts;
    std::list<std::shared_ptr<IMemoryAccessor>> _accessors;
    uint16_t _stackOffset;

};

}

#endif //NESCORE_MEMORY_H
