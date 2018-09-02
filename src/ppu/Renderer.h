#ifndef NESCORE_RENDERER_H
#define NESCORE_RENDERER_H

#include <memory>
#include <string>
#include "../memory/accessors/IMemoryAccessor.h"

namespace nescore
{

class PPUMemory;

class Renderer
{
public:
    static const uint32_t COLORS[0x40];

    static uint32_t getColor32Bit(uint8_t color);

public:
    Renderer(int width, int heigt);

    void setPattersSource(const IMemoryAccessor* accessor);
    void setPattern(uint16_t pattern, uint8_t row);
    void setAttributes(uint8_t attributes);
    void setPalette(uint8_t* palette);
    void render(int x, int y, int scrollX = 0, int scrollY = 0);
    void renderPattern(uint16_t pattern, int x, int y, int scrollX = 0, int scrollY = 0);
    void renderPatternTables();
    void swapBuffers();

    int getWidth() const;
    int getHeight() const;
    const uint32_t* getOutput() const;

    void saveToFile(const std::string& fileName);

private:
    int _bufferSize;
    int _width;
    int _height;
    uint32_t* _buffer1;
    uint32_t* _buffer2;
    uint32_t* _outputBuffer;
    uint8_t _plane0;
    uint8_t _plane1;
    uint8_t _attributes;
    uint8_t* _palette;
    const IMemoryAccessor* _patternsSource;

};

}


#endif //NESCORE_RENDERER_H
