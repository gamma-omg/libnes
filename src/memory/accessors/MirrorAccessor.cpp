#include "MirrorAccessor.h"

namespace nescore
{

MirrorAccessor::MirrorAccessor(IMemoryAccessor *source, Memory::Range range)
    : _source(source)
    , _range(range)
{
}

void MirrorAccessor::writeByte(uint16_t offset, uint8_t value)
{
    _source->writeByte(_range.getGlobalOffset(offset), value);
}

uint8_t MirrorAccessor::readByte(uint16_t offset)
{
    return _source->readByte(_range.getGlobalOffset(offset));
}

}
