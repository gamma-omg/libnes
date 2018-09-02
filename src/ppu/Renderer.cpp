#include <memory.h>
#include <fstream>
#include "Renderer.h"

namespace nescore
{

Renderer::Renderer(int width, int heigt)
    : _width(width)
    , _height(heigt)
    , _buffer1(new uint32_t[width * heigt])
    , _buffer2(new uint32_t[width * heigt])
    , _plane0(0)
    , _plane1(0)
{
    memset(_buffer1, 0x00, sizeof(uint32_t) * width * heigt);
    memset(_buffer2, 0x00, sizeof(uint32_t) * width * heigt);

    _outputBuffer = _buffer1;
}

void Renderer::setPattersSource(const IMemoryAccessor *accessor)
{
    _patternsSource = accessor;
}

void Renderer::setPattern(uint16_t pattern, uint8_t row)
{
    uint16_t baseOffset = pattern * 16;
    _plane0 = _patternsSource->readByte(baseOffset + row);
    _plane1 = _patternsSource->readByte(baseOffset + row + 8);
}

void Renderer::render(int x, int y, int scrollX, int scrollY)
{
    auto output = _outputBuffer + x + y * _width;
    for (int i = 7; i >= 0; --i)
    {
        uint8_t l = (_plane0 & (1 << i)) >> i;
        uint8_t h = (_plane1 & (1 << i)) >> i;
        *output++ = (h << 1) | l;
    }
}

void Renderer::renderPattern(uint16_t pattern, int x, int y, int scrollX, int scrollY)
{
    for (int row = 0; row < 8; ++row)
    {
        setPattern(pattern, row);
        render(x, y + row, scrollX, scrollY);
    }
}

void Renderer::swapBuffers()
{
    _outputBuffer = _outputBuffer == _buffer1 ? _buffer2 : _buffer1;
}

int Renderer::getWidth() const
{
    return _width;
}

int Renderer::getHeight() const
{
    return _height;
}

const uint32_t *Renderer::getOutput() const
{
    return _outputBuffer == _buffer1 ? _buffer2 : _buffer1;
}

void Renderer::saveToFile(const std::string &fileName)
{
    std::ofstream tgaFile(fileName.c_str(), std::ios::binary);

    uint8_t header[18] = { 0 };
    header[2] = 2;
    header[12] = _width & 0xFF;
    header[13] = (_width >> 8) & 0xFF;
    header[14] =  _height & 0xFF;
    header[15] = (_height >> 8) & 0xFF;
    header[16] = 24;

    tgaFile.write(reinterpret_cast<char*>(header), 18);

    const uint32_t* output = getOutput();
    for (int y = _height - 1; y >= 0; y--)
    {
        for (int x = 0; x < _width; x++)
        {
            uint32_t val = output[y * _width + x];
            uint32_t pixel = (val == 0) ? 0 : val == 1 ? 0xFF0000 : val == 2 ? 0x00FF00 : 0x0000FF;
            uint8_t r = (pixel & 0xFF0000) >> 16;
            uint8_t g = (pixel & 0x00FF00) >> 8;
            uint8_t b = pixel & 0x0000FF;

            tgaFile.put(r);
            tgaFile.put(g);
            tgaFile.put(b);
        }
    }

    tgaFile.close();
}

}