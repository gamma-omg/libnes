#ifndef NESCORE_IROMMAPPER_H
#define NESCORE_IROMMAPPER_H

#include <memory>

namespace nescore
{

class IMemoryAccessor;
class INESRom;
class Memory;

class IRomMapper
{
public:
    virtual ~IRomMapper() {}
    virtual std::shared_ptr<IMemoryAccessor> getCPUMemory() = 0;
    virtual std::shared_ptr<IMemoryAccessor> getPPUMemory() = 0;
};

}

#endif //NESCORE_IROMMAPPER_H
