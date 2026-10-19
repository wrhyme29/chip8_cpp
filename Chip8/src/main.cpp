#include <iostream>
#include <memory>

#include "Chip8MemoryMap.h"

void render_byte(uint8_t byteToRender)
{
    uint8_t updatedByteToRender = byteToRender >> 4;
    uint8_t masks[4] = {8, 4, 2, 1};
    for(uint8_t i=0; i < 4; ++i)
    {
      if(updatedByteToRender & masks[i])
      {
        printf("*");
      } else
      {
        printf(" ");
      }
    }
}

void render_sprite(uint8_t* sprite)
{
  for(uint8_t i=0; i < 5; ++i)
  {
    render_byte(sprite[i]);
    printf("\n");
  }
}

int main() {
  std::unique_ptr<Chip8MemoryMap> memoryMap = std::make_unique<Chip8MemoryMap>();
    
  std::printf("Constructed Chip8MemoryMap with size of %zu bytes\n", sizeof(*memoryMap));
  std::printf("Constants:\nRAM_START=0x%03X\nPROGRAM_START=0x%03X\nETI_PROGRAM_START=0x%03X\nRAM_END=0x%03X\n\n\n", 
    Chip8MemoryMap::address_boundaries::RAM_START, 
    Chip8MemoryMap::address_boundaries::PROGRAM_START, 
    Chip8MemoryMap::address_boundaries::ETI_PROGRAM_START, 
    Chip8MemoryMap::address_boundaries::RAM_END);

  uint8_t* sprite;
  for(uint8_t i=0; i < 16; ++i)
  {
    sprite = memoryMap->GetHexSprite(i);
    render_sprite(sprite);
    printf("\n");
  }
    
  return 0;
}
