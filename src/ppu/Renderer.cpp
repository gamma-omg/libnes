#include <memory.h>
#include <fstream>
#include "Renderer.h"

namespace nescore
{

const uint32_t Renderer::COLORS[0x40] = {
    0xFF7C7C7C, 0xFF0000FC, 0xFF0000BC, 0xFF4428BC, 0xFF940084, 0xFFA80020, 0xFFA81000, 0xFF881400, 0xFF503000, 0xFF007800,
    0xFF006800, 0xFF005800, 0xFF004058, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFBCBCBC, 0xFF0078F8, 0xFF0058F8, 0xFF6844FC,
    0xFFD800CC, 0xFFE40058, 0xFFF83800, 0xFFE45C10, 0xFFAC7C00, 0xFF00B800, 0xFF00A800, 0xFF00A844, 0xFF008888, 0xFF000000,
    0xFF000000, 0xFF000000, 0xFFF8F8F8, 0xFF3CBCFC, 0xFF6888FC, 0xFF9878F8, 0xFFF878F8, 0xFFF85898, 0xFFF87858, 0xFFFCA044,
    0xFFF8B800, 0xFFB8F818, 0xFF58D854, 0xFF58F898, 0xFF00E8D8, 0xFF787878, 0xFF000000, 0xFF000000, 0xFFFCFCFC, 0xFFA4E4FC,
    0xFFB8B8F8, 0xFFD8B8F8, 0xFFF8B8F8, 0xFFF8A4C0, 0xFFF0D0B0, 0xFFFCE0A8, 0xFFF8D878, 0xFFD8F878, 0xFFB8F8B8, 0xFFB8F8D8,
    0xFF00FCFC, 0xFFF8D8F8, 0xFF000000, 0xFF000000
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
        _outputBuffer[output++] = getColor32Bit(_palette[colorIndex]);
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
            uint32_t pixel = output[y * _width + x];
            uint8_t r = (pixel & 0xFF000000) >> 24;
            uint8_t g = (pixel & 0xFF0000) >> 16;
            uint8_t b = (pixel & 0xFF00) >> 8;

            tgaFile.put(b);
            tgaFile.put(g);
            tgaFile.put(r);
        }
    }

    tgaFile.close();
}

}