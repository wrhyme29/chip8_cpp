#include <memory>

#include "gtest/gtest.h"
#include "../src/Chip8Interpreter.h"

TEST(Chip8InterpreterTests, AddressBoundariesEnum)
{
    EXPECT_EQ(Chip8Interpreter::address_boundaries::RAM_START, 0x000);
    EXPECT_EQ(Chip8Interpreter::address_boundaries::PROGRAM_START, 0x200);
    EXPECT_EQ(Chip8Interpreter::address_boundaries::ETI_PROGRAM_START, 0x600);
    EXPECT_EQ(Chip8Interpreter::address_boundaries::VARS_DISPLAY_REFRESH, 0xE90);
    EXPECT_EQ(Chip8Interpreter::address_boundaries::RAM_END, 0xFFF);
}

TEST(Chip8InterpreterTests, SimpleAddingProgram)
{
    uint16_t program[4] = {
        0x6020, // Load V0 with 0x20
        0x7012, // Add V0 to 0x12
        0x00FD  // Exit
    };

    std::unique_ptr<Chip8Interpreter> m_chip8Interpreter = std::make_unique<Chip8Interpreter>();
    m_chip8Interpreter->LoadProgramIntoMemory(program, sizeof program);
    m_chip8Interpreter->ExecuteProgram();
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0x0), (uint8_t) 0x32);
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0xf), (uint8_t) 0x00);
}

TEST(Chip8InterpreterTests, SimpleAddingProgramWithCarry)
{
    uint16_t program[4] = {
        0x6096, // Load V0 with 0x20
        0x70a4, // Add V0 to 0x12
        0x00FD  // Exit
    };

    std::unique_ptr<Chip8Interpreter> m_chip8Interpreter = std::make_unique<Chip8Interpreter>();
    m_chip8Interpreter->LoadProgramIntoMemory(program, sizeof program);
    m_chip8Interpreter->ExecuteProgram();
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0x0), (uint8_t) 0x3a);
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0xf), (uint8_t) 0x00);
}

TEST(Chip8InterpreterTests, SimpleAddingRegistersProgram)
{
    uint16_t program[4] = {
        0x6020, // Load V0 with 0x20
        0x6112, // Load V1 with 0x12
        0x8014, // Add V0 to V1
        0x00FD  // Exit
    };

    std::unique_ptr<Chip8Interpreter> m_chip8Interpreter = std::make_unique<Chip8Interpreter>();
    m_chip8Interpreter->LoadProgramIntoMemory(program, sizeof program);
    m_chip8Interpreter->ExecuteProgram();
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0x0), (uint8_t) 0x32);
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0xf), (uint8_t) 0x00);
}

TEST(Chip8InterpreterTests, SimpleAddingRegistersProgramWithCarry)
{
    uint16_t program[4] = {
        0x6a96, // Load Va with 0x96
        0x6ba4, // Load Vb with 0xa4
        0x8ab4, // Add Va to Vb
        0x00FD  // Exit
    };

    std::unique_ptr<Chip8Interpreter> m_chip8Interpreter = std::make_unique<Chip8Interpreter>();
    m_chip8Interpreter->LoadProgramIntoMemory(program, sizeof program);
    m_chip8Interpreter->ExecuteProgram();
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0xa), (uint8_t) 0x3a);
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0xf), (uint8_t) 0x01);
}

TEST(Chip8InterpreterTests, SimpleSubtractRegistersProgram)
{
    uint16_t program[4] = {
        0x6a20, // Load Va with 0x96
        0x6b12, // Load Vb with 0xa4
        0x8ab5, // Sub Vb from Va
        0x00FD  // Exit
    };

    std::unique_ptr<Chip8Interpreter> m_chip8Interpreter = std::make_unique<Chip8Interpreter>();
    m_chip8Interpreter->LoadProgramIntoMemory(program, sizeof program);
    m_chip8Interpreter->ExecuteProgram();
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0xa), (uint8_t) 0x0e);
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0xf), (uint8_t) 0x01);
}

TEST(Chip8InterpreterTests, SimpleSubtractRegistersProgramWithBorrow)
{
    uint16_t program[4] = {
        0x6a12, // Load Va with 0x96
        0x6b20, // Load Vb with 0xa4
        0x8ab5, // Sub Vb from Va
        0x00FD  // Exit
    };

    std::unique_ptr<Chip8Interpreter> m_chip8Interpreter = std::make_unique<Chip8Interpreter>();
    m_chip8Interpreter->LoadProgramIntoMemory(program, sizeof program);
    m_chip8Interpreter->ExecuteProgram();
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0xa), (uint8_t) 0x0e);
    EXPECT_EQ(m_chip8Interpreter->GetRegistryValue(0xf), (uint8_t) 0x00);
}
