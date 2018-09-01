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
    std::shared_ptr<IRomMapper> createMapper(std::shared_ptr<INESRom> rom)
    {
        switch (rom->getMapper())
        {
            case MapperNROM: return std::make_shared<NROM>(rom);
        }

        return nullptr;
    }

};

}

#endif //NESCORE_MAPPERFACTORY_H
