#include <cassert>
#include <cstdio>
#include "Chip8Interpreter.h"

// Following RAII, allocate the buffers on construction
Chip8Interpreter::Chip8Interpreter()
{
    m_memory = new uint8_t[CHIP8_RAM_SIZE_BYTES];
    m_registers = new uint8_t[CHIP8_REGISTERS_COUNT];
    m_specialRegisters = std::make_unique<SpecialRegisters>();
    m_stack = new uint16_t[CHIP8_STACK_ADDRESS_COUNT];
    LoadProgramIntoMemory(c_hexSprites, sizeof c_hexSprites, address_boundaries::RAM_START);
}

// Following RAII, deallocate the buffers on destruction
Chip8Interpreter::~Chip8Interpreter()
{
    delete[] m_stack;
    delete[] m_registers;
    delete[] m_memory;
}

void Chip8Interpreter::ExecuteProgram()
{
    uint8_t words[4];
    uint16_t result;
    uint16_t program_counter = address_boundaries::PROGRAM_START;
    while(true)
    {
        words[0] = m_memory[program_counter + 0];
        words[1] = m_memory[program_counter + 1];
        words[2] = m_memory[program_counter + 2];
        words[3] = m_memory[program_counter + 3];
        program_counter += 4;

        switch(words[0])
        {
            case 0x0:
                // 0x00FD is EXIT
                if( words[1] == 0x0 && words[2] == 0xf && words[3] == 0xd) return;
                break;
            case 0x6:
                // 0x6xkk is Load kk into Vx          
                m_registers[words[1]] = static_cast<uint8_t>((words[2] << 4) + words[3]);
                break;
            case 0x7:
                // 0x7xkk is Set Vx to Vx + kk.
                m_registers[words[1]] = static_cast<uint8_t>(m_registers[words[1]] + static_cast<uint8_t>((words[2] << 4) + words[3]));
                break;
            case 0x8:
                switch(words[3])
                {
                    case 0x4:
                        // 0x8xy4 is Set Vx = Vx + Vy, set VF = carry.
                        result = m_registers[words[1]] + m_registers[words[2]];
                        if(result > 0xff)
                        {
                            m_registers[0xf] = static_cast<uint8_t>(0x1);
                            result &= 0xff;
                        } else
                        {
                            m_registers[0xf] = static_cast<uint8_t>(0x0);
                        }
                        m_registers[words[1]] = static_cast<uint8_t>(result);
                        break;
                    case 0x5:
                        // 0x8xy5 is Set Vx = Vx - Vy, set VF = not borrow.
                        if(m_registers[words[1]] >= m_registers[words[2]])
                        {
                            m_registers[words[1]] = static_cast<uint8_t>(m_registers[words[1]] - m_registers[words[2]]);
                            m_registers[0xf]      = static_cast<uint8_t>(0x1);
                        } else
                        {
                            m_registers[words[1]] = static_cast<uint8_t>(m_registers[words[2]] - m_registers[words[1]]);
                            m_registers[0xf]      = static_cast<uint8_t>(0x0);
                        }
                        break;
                    case 0x7:
                        // 0x8xy7 is Set Vx = Vx - Vy, set VF = not borrow.
                        if(m_registers[words[1]] >= m_registers[words[2]])
                        {
                            m_registers[words[1]] = static_cast<uint8_t>(m_registers[words[1]] - m_registers[words[2]]);
                            m_registers[0xf]      = static_cast<uint8_t>(0x1);
                        } else
                        {
                            m_registers[words[1]] = static_cast<uint8_t>(m_registers[words[2]] - m_registers[words[1]]);
                            m_registers[0xf]      = static_cast<uint8_t>(0x0);
                        }
                        break;
                }
                break;

        }
    }
}

uint8_t* Chip8Interpreter::GetHexSprite(uint8_t hexChar)
{
    assert(hexChar < 0x10);
    uint8_t* hexSprite = new uint8_t[5];

    uint16_t memIndex;
    for(uint8_t i=0; i < 5; ++i)
    {
        memIndex = address_boundaries::RAM_START + hexChar * 5 + i;
        hexSprite[i] = m_memory[memIndex];
    }

    return hexSprite;
}

uint8_t Chip8Interpreter::GetRegistryValue(uint8_t registerName)
{
    return m_registers[registerName];
}

void Chip8Interpreter::LoadProgramIntoMemory(const uint16_t* program, uint16_t size)
{
    uint8_t words[4];
    for(int i=0; i < size; ++i)
    {
        words[0] = (program[i] & 0xf000) >> 12;
        words[1] = (program[i] & 0x0f00) >> 8;
        words[2] = (program[i] & 0x00f0) >> 4;
        words[3] = (program[i] & 0x000f) >> 0;


        for(int j=0; j < 4; ++j)
        {
            m_memory[address_boundaries::PROGRAM_START + i * 4 + j] = words[j];
        }
    }
}


void Chip8Interpreter::LoadProgramIntoMemory(const uint8_t* program, uint16_t size, uint16_t startMemoryAddress)
{
    assert(startMemoryAddress + size <= address_boundaries::RAM_END);
    for(int i=0; i < size; ++i)
    {
        m_memory[startMemoryAddress + i] = program[i];
    }
}

void Chip8Interpreter::SetRegisterI(uint16_t memoryAddress)
{
    assert(memoryAddress <= address_boundaries::RAM_END);
    m_specialRegisters->I = memoryAddress;
}
