#include <cassert>

#include "Chip8MemoryMap.h"

// Following RAII, allocate the buffer on construction
Chip8MemoryMap::Chip8MemoryMap() : 
    m_size(4096)
{
    m_buffer = new uint8_t[m_size];
    LoadHexSpritesIntoMemory();
}

// Following RAII, deallocate the buffer on destruction
Chip8MemoryMap::~Chip8MemoryMap()
{
    delete[] m_buffer;
}

uint8_t* Chip8MemoryMap::GetHexSprite(uint8_t hexChar)
{
    assert(hexChar < 0xF);
    uint8_t* hexSprite = new uint8_t[5];

    uint16_t memIndex;
    for(uint8_t i=0; i < 5; ++i)
    {
        memIndex = address_boundaries::RAM_START + hexChar * 5 + i;
        hexSprite[i] = m_buffer[memIndex];
    }

    return hexSprite;
}

void Chip8MemoryMap::LoadHexSpritesIntoMemory()
{
    for(int i=0; i < sizeof c_hexSprites; ++i)
    {
        m_buffer[i] = c_hexSprites[i];
    }
}