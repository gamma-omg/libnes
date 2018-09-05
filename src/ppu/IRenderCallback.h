#ifndef NESCORE_IRENDERCALLBACK_H
#define NESCORE_IRENDERCALLBACK_H

#include <cstdint>

namespace nescore
{

class IRenderCallback
{
public:
    virtual ~IRenderCallback();
    virtual void renderFrame(int width, int height, const uint32_t * frameBuffer) = 0;
};

}

#endif //NESCORE_IRENDERCALLBACK_H
