#include <gtest/gtest.h>
#include <ppu/Renderer.h>
#include <rom/INESRom.h>
#include <fstream>

using namespace nescore;

TEST(RENDERER, CHR_BattleCity)
{
    INESRom rom;
    std::ifstream stream("tests/data/games/battlecity.nes", std::ios::binary);
    stream >> rom;

    Renderer renderer(128, 256);
    renderer.setPattersSource(rom.getChrRomBank(0));

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            renderer.renderPattern(j + i * 16, 8 * j, i * 8);
        }
    }
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            renderer.renderPattern(256 + j + i * 16, 8 * j, i * 8 + 128);
        }
    }

    renderer.swapBuffers();
    renderer.saveToFile("tests/data/games/battlecity.tga");
}

TEST(RENDERER, CHR_DonkeyKong)
{
    INESRom rom;
    std::ifstream stream("tests/data/games/donkey_kong.nes", std::ios::binary);
    stream >> rom;

    Renderer renderer(128, 256);
    renderer.setPattersSource(rom.getChrRomBank(0));

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            renderer.renderPattern(j + i * 16, 8 * j, i * 8);
        }
    }
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            renderer.renderPattern(256 + j + i * 16, 8 * j, i * 8 + 128);
        }
    }

    renderer.swapBuffers();
    renderer.saveToFile("tests/data/games/donkey_kong.tga");
}
