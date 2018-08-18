#ifndef NESCORE_IROMMAPPER_H
#define NESCORE_IROMMAPPER_H

#include <memory>

namespace nescore
{

class Memory;
class INESRom;

class IRomMapper
{
public:
    IRomMapper(std::shared_ptr<Memory> memory) : _memory(memory) { }

    virtual void map(INESRom& rom) = 0;

protected:
    std::shared_ptr<Memory> _memory;
};

}

#endif //NESCORE_IROMMAPPER_H
