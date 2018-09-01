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
    virtual void setupCPU(std::shared_ptr<Memory> memory) = 0;
    virtual void setupPPU(std::shared_ptr<Memory> memory) = 0;
};

}

#endif //NESCORE_IROMMAPPER_H
