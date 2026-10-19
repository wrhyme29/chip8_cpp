#include <iostream>
#include <memory>
#include "Chip8MemoryMap.h"

int main() {
  std::unique_ptr<Chip8MemoryMap> memoryMap;
    
  std::printf("Constructed Chip8MemoryMap with size of %zu bytes\n", sizeof(*memoryMap));
  std::printf("Constants:\nRAM_START=0x%03X\nPROGRAM_START=0x%03X\nETI_PROGRAM_START=0x%03X\nRAM_END=0x%03X\n", 
    Chip8MemoryMap::address_boundaries::RAM_START, 
    Chip8MemoryMap::address_boundaries::PROGRAM_START, 
    Chip8MemoryMap::address_boundaries::ETI_PROGRAM_START, 
    Chip8MemoryMap::address_boundaries::RAM_END);
    
  return 0;
}
