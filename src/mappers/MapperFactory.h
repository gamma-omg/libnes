#ifndef NESCORE_MAPPERFACTORY_H
#define NESCORE_MAPPERFACTORY_H

#include <memory>
#include "NROM.h"

namespace nescore
{

class MapperFactory
{
public:
    enum MapperTypes
    {
        MapperNROM = 0
    };

public:
    MapperFactory(std::shared_ptr<Memory> memory)
        : _memory(memory)
    {
    }

    std::shared_ptr<IRomMapper> createMapper(uint8_t mapperType)
    {
        switch (mapperType)
        {
            case MapperNROM:
                return std::make_shared<NROM>(_memory);
        }

        return nullptr;
    }

private:
    std::shared_ptr<Memory> _memory;
};

}

#endif //NESCORE_MAPPERFACTORY_H
