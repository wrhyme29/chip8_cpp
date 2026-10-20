###################################
#    Chip8 Executable
###################################

add_executable(chip8
    "src/main.cpp"
)

add_library(chip8-lib STATIC
    "src/Chip8Interpreter.h"
    "src/Chip8Interpreter.cpp"
)

target_link_libraries(chip8 chip8-lib)

install(
    TARGETS chip8 
    RUNTIME DESTINATION bin
)


###################################
#    Chip8Test Executable
###################################

add_executable(chip8-test
  "tests/Chip8InterpreterTests.cpp"
)

target_link_libraries(chip8-test chip8-lib gtest gtest_main)
