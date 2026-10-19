#include "gtest/gtest.h"
#include "../src/Chip8MemoryMap.h"

TEST(Chip8MemoryMapTests, AddressBoundariesEnum)
{
    EXPECT_EQ(Chip8MemoryMap::address_boundaries::RAM_START, 0x000);
    EXPECT_EQ(Chip8MemoryMap::address_boundaries::PROGRAM_START, 0x200);
    EXPECT_EQ(Chip8MemoryMap::address_boundaries::ETI_PROGRAM_START, 0x600);
    EXPECT_EQ(Chip8MemoryMap::address_boundaries::RAM_END, 0xFFF);
}

