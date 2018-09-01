#include "Memory.h"
#include "accessors/IMemoryAccessor.h"
#include "accessors/BufferAccessor.h"
#include "accessors/MirrorAccessor.h"

namespace nescore
{

Memory::Mount::Mount(Memory::Range range, IMemoryAccessor *accessor)
    : range(range)
    , accessor(accessor)
{
}

Memory::Range::Range(uint16_t start, uint16_t end)
    : start(start)
    , end(end)
{
}

bool Memory::Range::contains(uint16_t offset)
{
    return offset >= start && offset <= end;
}

uint16_t Memory::Range::getGlobalOffset(uint16_t offset)
{
    return (start + offset) % (end - start);
}


Memory::Memory()
    : _stackOffset(0)
{
}

uint8_t Memory::readByte(uint16_t offset)
{
    auto mount = findMount(_readMounts, offset);
    if (!mount)
    {
        throw nes_memory_error("Mount point was not found for address " + offset);
    }

    return mount->accessor->readByte(offset - mount->range.start);
}

void Memory::writeByte(uint16_t offset, uint8_t value)
{
    auto mount = findMount(_writeMounts, offset);
    if (!mount)
    {
        throw nes_memory_error("Mount point was not found for address " + offset);
    }

    mount->accessor->writeByte(offset - mount->range.start, value);
}

uint8_t Memory::popByte(uint8_t &s)
{
    s++;
    return readByte(_stackOffset + s);
}

uint16_t Memory::readShort(uint16_t offset)
{
    uint8_t l = readByte(offset);
    uint8_t h = readByte(offset + 1);
    return l | (h << 8);
}

uint16_t Memory::popShort(uint8_t &s)
{
    uint8_t l = popByte(s);
    uint8_t h = popByte(s);
    return (h << 8) | l;
}

void Memory::writeShort(uint16_t offset, uint16_t value)
{
    writeByte(offset, value & 0xFF);
    writeByte(offset + 1, value >> 8);
}

void Memory::pushByte(uint8_t &s, uint8_t value)
{
    writeByte(_stackOffset + s, value);
    s--;
}

void Memory::pushShort(uint8_t &s, uint16_t value)
{
    uint8_t l = value & 0xFF;
    uint8_t h = value >> 8;
    pushByte(s, h);
    pushByte(s, l);
}

void Memory::readBytes(IMemoryAccessor* dst, uint16_t offset, uint16_t size)
{
    for (auto i = offset; i < offset + size; ++i)
    {
        dst->writeByte(i, readByte(i));
    }
}

void Memory::readBytes(uint8_t *dst, uint16_t offset, uint16_t size)
{
    for (int i = offset; i < offset + size; ++i)
    {
        *dst++ = readByte(i);
    }
}

void Memory::writeBytes(IMemoryAccessor* src, uint16_t offset, uint16_t size)
{
    for (auto i = 0; i < size; ++i)
    {
        writeByte(offset + i, src->readByte(i));
    }
}

std::string Memory::readString(uint16_t offset)
{
    char buffer[0x2000];
    char chr;
    int address = 0;
    do
    {
        chr = readByte(offset + address);
        buffer[address++] = chr;
        if (address >= 0x2000)
        {
            buffer[address - 1] = '\0';
        }
    }
    while (chr != '\0');

    return std::string(buffer);
}

void Memory::mount(Memory::Range range, IMemoryAccessor *accessor, MountMode mode)
{
    if (mode & MountMode::Read)
    {
        _readMounts.emplace_front(Mount(range, accessor));
    }
    if (mode & MountMode::Write)
    {
        _writeMounts.emplace_front(Mount(range, accessor));
    }
}

void Memory::mount(Memory::Range range, uint8_t* buffer, MountMode mode)
{
    auto accessor = std::make_shared<BufferAccessor>();
    accessor->setBuffer(buffer);
    mount(range, accessor.get(), mode);

    _accessors.push_back(accessor);
}

void Memory::mirror(Memory::Range src, Memory::Range dst, MountMode mode)
{
    auto accessor = std::make_shared<MirrorAccessor>(this, src);
    mount(dst, accessor.get(), mode);

    _accessors.push_back(accessor);
}

void Memory::unmountAll()
{
    _readMounts.clear();
    _writeMounts.clear();
    _accessors.clear();
}

void Memory::setStackOffset(uint16_t offset)
{
    _stackOffset = offset;
}

Memory::Mount* Memory::findMount(std::list<Mount>& source, uint16_t offset)
{
    for (auto& mount : source)
    {
        if (mount.range.contains(offset))
        {
            return &mount;
        }
    }

    return nullptr;
}

}