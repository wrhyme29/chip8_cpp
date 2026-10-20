#include <iostream>
#include <memory>

#include "Chip8Interpreter.h"

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
  std::unique_ptr<Chip8Interpreter> interpreter = std::make_unique<Chip8Interpreter>();
    
  std::printf("Constructed Chip8MemoryMap with size of %zu bytes\n", sizeof(*interpreter));

  uint8_t* sprite;
  for(uint8_t i=0; i < 16; ++i)
  {
    sprite = interpreter->GetHexSprite(i);
    render_sprite(sprite);
    printf("\n");
  }
    
  return 0;
}
