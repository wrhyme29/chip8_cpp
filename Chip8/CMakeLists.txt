###################################
#    Chip8 Executable
###################################

add_executable(Chip8
    "src/main.cpp"
)

add_library(Chip8_Internal STATIC
    "src/Chip8MemoryMap.h"
    "src/Chip8MemoryMap.cpp"
)

target_link_libraries(Chip8 Chip8_Internal)


###################################
#    Chip8Test Executable
###################################

add_executable(Chip8Test
  "tests/Chip8MemoryMapTests.cpp"
)

target_link_libraries(Chip8Test Chip8_Internal gtest gtest_main)
