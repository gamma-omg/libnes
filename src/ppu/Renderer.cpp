#include <memory.h>
#include <fstream>
#include "Renderer.h"

namespace nescore
{

const uint32_t Renderer::COLORS[0x40] = {
    0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400, 0x503000, 0x007800,
    0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000, 0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC,
    0xD800CC, 0xE40058, 0xF83800, 0xE45C10, 0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000,
    0x000000, 0x000000, 0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044,
    0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000, 0xFCFCFC, 0xA4E4FC,
    0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8, 0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8,
    0x00FCFC, 0xF8D8F8, 0x000000, 0x000000
};

inline uint32_t Renderer::getColor32Bit(uint8_t color)
{
    return COLORS[color];
}

Renderer::Renderer(int width, int heigt)
    : _bufferSize(width * heigt)
    , _width(width)
    , _height(heigt)
    , _buffer1(new uint32_t[_bufferSize])
    , _buffer2(new uint32_t[_bufferSize])
    , _plane0(0)
    , _plane1(0)
    , _attributes(0)
    , _palette(nullptr)
{
    memset(_buffer1, 0x00, sizeof(uint32_t) * _bufferSize);
    memset(_buffer2, 0x00, sizeof(uint32_t) * _bufferSize);

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

void Renderer::setAttributes(uint8_t attributes)
{
    _attributes = attributes;
}

void Renderer::setPalette(uint8_t *palette)
{
    _palette = palette;
}

void Renderer::render(int x, int y, int scrollX, int scrollY)
{
    auto output = x + y * _width;
    for (int i = 7; i >= 0; --i)
    {
        if (output >= _bufferSize)
        {
            return;
        }

        uint8_t l = (_plane0 & (1 << i)) >> i;
        uint8_t h = (_plane1 & (1 << i)) >> i;
        uint8_t colorIndex = (h << 1) | l | (_attributes << 4);
        _outputBuffer[output++] = _palette[colorIndex];
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

void Renderer::renderPatternTables()
{
    int x = 0;
    int y = 0;
    for (int pattern = 0; pattern < 512; ++pattern)
    {
        renderPattern(pattern, x, y);
        x += 8;
        if (x >= 128)
        {
            x = 0;
            y += 8;
        }
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
            uint32_t color8 = output[y * _width + x];
            uint32_t color32 = getColor32Bit(color8);
            uint8_t r = (color32 & 0xFF0000) >> 16;
            uint8_t g = (color32 & 0xFF00) >> 8;
            uint8_t b = color32 & 0xFF;

            tgaFile.put(b);
            tgaFile.put(g);
            tgaFile.put(r);
        }
    }

    tgaFile.close();
}

}