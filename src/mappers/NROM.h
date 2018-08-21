#ifndef NESCORE_NROM_H
#define NESCORE_NROM_H

#include "IRomMapper.h"

namespace nescore
{

class NROM : public IRomMapper
{
public:
    NROM(std::shared_ptr<Memory> memory);
    void map(INESRom &rom) override;
};

}

#endif //NESCORE_NROM_H
