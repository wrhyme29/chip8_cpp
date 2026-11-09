#include <iostream>
#include <memory>

#include "Chip8Interpreter.h"
#include "Sdl2Renderer.h"

void render_byte(uint8_t byteToRender)
{
    uint8_t masks[8] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x04, 0x02, 0x01};
    for(uint8_t i=0; i < 8; ++i)
    {
      if(byteToRender & masks[i])
      {
        printf("#");
      } else
      {
        printf(".");
      }
    }
}

void render_bit(bool bit)
{
      if(bit)
      {
        printf("#");
      } else
      {
        printf(".");
      }
}

void render_screen(bool** screen)
{
  printf("\n");
  for(int x=0; x < CHIP8_SCREEN_HEIGHT; ++x)
  {
    for(int y=0; y < CHIP8_SCREEN_WIDTH; ++y)
    {
      render_bit(screen[y][x]);
    }
    printf("\n");
  }
  printf("\n");
}

void render_sprite(uint8_t* sprite)
{
  for(uint8_t i=0; i < 5; ++i)
  {
    render_byte(sprite[i]);
    printf("\n");
  }
}

int main(int argc, char** args) {
  std::unique_ptr<Chip8Interpreter> interpreter = std::make_unique<Chip8Interpreter>();
  std::unique_ptr<Sdl2Renderer> sdl2_renderer = std::make_unique<Sdl2Renderer>(CHIP8_SCREEN_WIDTH, CHIP8_SCREEN_HEIGHT, 10, 60);
  if(!sdl2_renderer->IsValid()) return 1;
    
  // std::printf("Constructed Chip8MemoryMap with size of %zu bytes\n", sizeof(*interpreter));

  // uint8_t* sprite;
  // for(uint8_t i=0; i < 16; ++i)
  // {
  //   sprite = interpreter->GetHexSprite(i);
  //   render_sprite(sprite);
  //   printf("\n");
  // }

  // uint16_t program[12] = {
  //       0x00e0, // Clear the Screen
  //       0x6005, // Set x coord
  //       0x6109, // Set y coord
  //       0x6206, // Load V2 with digit to draw
  //       0xf229, // Point I to the font for the digit to draw
  //       0xd015, // Draw Sprite
  //       0x600c, // Set x coord
  //       0x610c, // Set y coord
  //       0x6209, // Load V2 with digit to draw
  //       0xf229, // Point I to the font for the digit to draw
  //       0xd015, // Draw Sprite
  //       0x00FD  // Exit
  // };

  // draw_faces.ch8
  //  uint16_t program[23] = {
  //       0x1224, // jump to 0x224
  //       0xff81, // sprite data
  //       0xa581, // sprite data
  //       0xbd81, // sprite data
  //       0xffff, // sprite data
  //       0x81e7, // sprite data
  //       0x8181, // sprite data
  //       0x99ff, // sprite data
  //       0xa202, // Set I to 0x202 - start subroutine
  //       0xd017, // Draw 7 byte sprite at (0, 1)
  //       0x7009, // Add 9 to register V0
  //       0x7102, // Add 2 to register V1
  //       0x00ee, // Return from subroutine
  //       0xa209, // Set I to 0x209 - start subroutine
  //       0xd017, // Draw 7 byte sprite at (0, 1)
  //       0x7009, // Add 9 to register V0
  //       0x7102, // Add 2 to register V1
  //       0x00ee, // Return from subroutine
  //       0x2210, // Call subroutine at 0x210
  //       0x2210, // Call subroutine at 0x210
  //       0x221a, // Call subroutine at 0x21a
  //       0x2210, // Call subroutine at 0x210
  //       0x00FD  // Exit
  //   };

  // // ball_moving.ch8
  uint16_t program[19] {
      0x120a, // Jump to 0x20a
      0x3c7a, // start sprite data
      0xfdff,
      0xffff,
      0x7e3c, // end sprite data
      0xa202, // Set I to 0x202
      0x6a20, // Set Va to 0x20
      0x6b0c, // Set Vb to 0x0c
      0x6d01, // Set Vd to 0x01
      0x00e0, // Clear the screen - start loop
      0xdab8, // Draw 8 byte sprite at (0xa, 0xb)
      0x7a01, // Set Va to Va + 1
      0x8bd4, // Set Vb to Vb + Vd
      0x4b18, // Skip next if Vb != 0x18
      0x6dff, // Set Vd to 0xff
      0x4b00, // Skip next if Vb != 0x0
      0x6d01, // Set Vd to 0x01
      0x1212, // jump to loop start at 0x212
      0x00fd, // Exit
  };

  std::function<void(bool**)> renderer = [&sdl2_renderer](bool** screen) {
        sdl2_renderer->RenderScreen(screen);
    };

  std::function<void(Chip8Event*, size_t*)> poller = [&sdl2_renderer](Chip8Event* events, size_t* count) {
        sdl2_renderer->PollEvents(events, count);
    };
  
  interpreter->SetRenderer(renderer);
  interpreter->SetEventPoller(poller);
  interpreter->LoadProgramIntoMemory(program, sizeof program);
  interpreter->ExecuteProgram();

  while(!sdl2_renderer->ShouldExit());

  return 0;
}
