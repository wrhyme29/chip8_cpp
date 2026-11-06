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
    m_screen = new bool*[CHIP8_SCREEN_WIDTH];
    for(int i = 0; i < CHIP8_SCREEN_WIDTH; ++i)
    {
        m_screen[i] = new bool[CHIP8_SCREEN_HEIGHT];
    }

}

// Following RAII, deallocate the buffers on destruction
Chip8Interpreter::~Chip8Interpreter()
{
    for(int i = 0; i < CHIP8_SCREEN_WIDTH; ++i)
    {
        delete[] m_screen[i];
    }
    delete[] m_screen;
    delete[] m_stack;
    delete[] m_registers;
    delete[] m_memory;
}

void ProcessInstruction(uint8_t word1, uint8_t word2, uint8_t word3, uint8_t word4)
{
 // TODO: Move the switch case for processing a word into here
 //       This will then allow 2 key things:
 //         1) The unit testing of certain instructions being processed correctly
 //         2) The interleaving of the program execution and screen rendering 
}

void Chip8Interpreter::ExecuteProgram()
{
    uint8_t words[4];
    uint16_t result;
    uint8_t byte, x_coord, y_coord, mask;
    uint16_t program_counter = address_boundaries::PROGRAM_START;
    bool pixel;
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
                // 0x00E0 is Clear the display
                if( words[1] == 0x0 && words[2] == 0xe && words[3] == 0x0)
                {
                    for(int x=0; x < CHIP8_SCREEN_WIDTH; ++x)
                    {
                        for(int y=0; y < CHIP8_SCREEN_HEIGHT; ++y)
                        {
                            m_screen[x][y] = static_cast<uint8_t>(0x0);
                        }
                    }
                }
                // 0x00FD is EXIT
                if( words[1] == 0x0 && words[2] == 0xf && words[3] == 0xd) return;
                break;
            case 0x1:
                // 0x1nnn is Jump to location nnn
                program_counter = static_cast<uint16_t>((words[1] << 8) + (words[2] << 4) + words[3]);
                break;
            case 0x3:
                // 0x3xkk is Skip next instruction if Vx = kk.
                if(m_registers[words[1]] == static_cast<uint8_t>((words[2] << 4) + words[3]))
                {
                    program_counter += 4;
                }
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
            case 0xa:
                // 0xannn is Set I to nnn
                m_specialRegisters->I = static_cast<uint16_t>((words[1] << 8) + (words[2] << 4) + words[3]);
                break;
            case 0xb:
                // 0xbnnn is Jump to location nnn + V0.
                program_counter = static_cast<uint16_t>(m_registers[0x0] + static_cast<uint16_t>((words[1] << 8) + (words[2] << 4) + words[3]));
                break;
            case 0xd:
                // 0xdxyn is Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
                m_registers[0xf] = 0x0;
                for(int i=0; i < words[3]; ++i)
                {
                    byte = m_memory[m_specialRegisters->I + i];
                    for(int j=0; j < 8; ++j)
                    {
                        x_coord = (m_registers[words[1]] + 8 - j) % CHIP8_SCREEN_WIDTH;
                        y_coord = (m_registers[words[2]] + i) % CHIP8_SCREEN_HEIGHT;
                        mask = 0x1 << j;
                        if((m_screen[x_coord][y_coord]) && !(byte & mask))
                        {
                            m_registers[0xf] = 0x1;
                        } 

                        pixel = m_screen[x_coord][y_coord] ^ (byte & mask);
                        m_screen[x_coord][y_coord] = pixel;
                    }
                }
                break;

            case 0xf:
                // 0xfx29 is Set I to location of sprite for digit Vx
                if(words[2] == 0x2 && words[3] == 0x9)
                {
                    m_specialRegisters->I = static_cast<uint16_t>(address_boundaries::RAM_START + m_registers[words[1]] * CHIP8_HEX_SPRITE_SIZE_BYTES);
                }
                break;
        }
        
        if(m_renderCallback != nullptr) m_renderCallback(m_screen);
    }
}


uint8_t* Chip8Interpreter::GetHexSprite(uint8_t hexChar)
{
    assert(hexChar < 0x10);
    uint8_t* hexSprite = new uint8_t[CHIP8_HEX_SPRITE_SIZE_BYTES];

    uint16_t memIndex;
    for(uint8_t i=0; i < CHIP8_HEX_SPRITE_SIZE_BYTES; ++i)
    {
        memIndex = address_boundaries::RAM_START + hexChar * CHIP8_HEX_SPRITE_SIZE_BYTES + i;
        hexSprite[i] = m_memory[memIndex];
    }

    return hexSprite;
}

uint8_t Chip8Interpreter::GetRegistryValue(uint8_t registerName)
{
    return m_registers[registerName];
}

bool** Chip8Interpreter::GetScreen()
{
    return m_screen;
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

void Chip8Interpreter::SetRenderer(std::function<void(bool**)> renderCallback)
{
    m_renderCallback = renderCallback;
}
