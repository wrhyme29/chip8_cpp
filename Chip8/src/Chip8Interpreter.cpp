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

void Chip8Interpreter::ProcessInstruction(uint8_t word1, uint8_t word2, uint8_t word3, uint8_t word4)
{
 // TODO: Move the switch case for processing a word into here
 //       This will then allow 2 key things:
 //         1) The unit testing of certain instructions being processed correctly
 //         2) The interleaving of the program execution and screen rendering 

    uint16_t result;
    uint8_t byte, x_coord, y_coord, mask;
    bool pixel;

     switch(word1)
        {
            case 0x0:
                // 0x00E0 is Clear the display
                if( word2 == 0x0 && word3 == 0xe && word4 == 0x0)
                {
                    for(int x=0; x < CHIP8_SCREEN_WIDTH; ++x)
                    {
                        for(int y=0; y < CHIP8_SCREEN_HEIGHT; ++y)
                        {
                            m_screen[x][y] = 0;
                        }
                    }
                }
                // 0x00ee is Return from a subroutine.
                if( word2 == 0x0 && word3 == 0xe && word4 == 0xe)
                {
                    // Check against stack underflow
                    assert(m_stackPointer > 0);
                    m_programCounter = m_stack[--m_stackPointer];
                }
                // 0x00FD is EXIT
                if( word2 == 0x0 && word3 == 0xf && word4 == 0xd) m_exit = true;
                break;
            case 0x1:
                // 0x1nnn is Jump to location nnn
                m_programCounter = static_cast<uint16_t>((word2 << 8) + (word3 << 4) + word4);
                break;
            case 0x2:
                // 0x2nnn is Call subroutine at nnn.

                // Check against stack overflow
                assert(m_stackPointer < CHIP8_STACK_ADDRESS_COUNT);
                m_stack[m_stackPointer++] = m_programCounter;
                m_programCounter = static_cast<uint16_t>((word2 << 8) + (word3 << 4) + word4);
                break;
            case 0x3:
                // 0x3xkk is Skip next instruction if Vx = kk.
                if(m_registers[word2] == static_cast<uint8_t>((word3 << 4) + word4))
                {
                    m_programCounter += 4;
                }
                break;
            case 0x4:
                // 0x4xkk is Skip next instruction if Vx != kk.
                if(m_registers[word2] != static_cast<uint8_t>((word3 << 4) + word4))
                {
                    m_programCounter += 4;
                }
                break;
            case 0x5:
                // 0x5xy0 is Skip next instruction if Vx = Vy.
                if(word4 == 0x0)
                {
                    if(m_registers[word2] == m_registers[word3])
                    {
                        m_programCounter += 4;
                    }
                }
                break;
            case 0x6:
                // 0x6xkk is Load kk into Vx          
                m_registers[word2] = static_cast<uint8_t>((word3 << 4) + word4);
                break;
            case 0x7:
                // 0x7xkk is Set Vx to Vx + kk.
                m_registers[word2] = static_cast<uint8_t>(m_registers[word2] + static_cast<uint8_t>((word3 << 4) + word4));
                break;
            case 0x8:
                switch(word4)
                {
                    case 0x4:
                        // 0x8xy4 is Set Vx = Vx + Vy, set VF = carry.
                        result = m_registers[word2] + m_registers[word3];
                        if(result > 0xff)
                        {
                            m_registers[0xf] = static_cast<uint8_t>(0x1);
                            result &= 0xff;
                        } else
                        {
                            m_registers[0xf] = static_cast<uint8_t>(0x0);
                        }
                        m_registers[word2] = static_cast<uint8_t>(result);
                        break;
                    case 0x5:
                        // 0x8xy5 is Set Vx = Vx - Vy, set VF = not borrow.
                        if(m_registers[word2] >= m_registers[word3])
                        {
                            m_registers[word2] = static_cast<uint8_t>(m_registers[word2] - m_registers[word3]);
                            m_registers[0xf]      = static_cast<uint8_t>(0x1);
                        } else
                        {
                            m_registers[word2] = static_cast<uint8_t>(m_registers[word3] - m_registers[word2]);
                            m_registers[0xf]      = static_cast<uint8_t>(0x0);
                        }
                        break;
                    case 0x7:
                        // 0x8xy7 is Set Vx = Vx - Vy, set VF = not borrow.
                        if(m_registers[word2] >= m_registers[word3])
                        {
                            m_registers[word2] = static_cast<uint8_t>(m_registers[word2] - m_registers[word3]);
                            m_registers[0xf]      = static_cast<uint8_t>(0x1);
                        } else
                        {
                            m_registers[word2] = static_cast<uint8_t>(m_registers[word3] - m_registers[word2]);
                            m_registers[0xf]      = static_cast<uint8_t>(0x0);
                        }
                        break;
                }
                break;
            case 0x9:
                // 0x9xy0 is Skip next instruction if Vx != Vy.
                if(word4 == 0x0)
                {
                    if(m_registers[word2] != m_registers[word3])
                    {
                        m_programCounter += 4;
                    }
                }
                break;
            case 0xa:
                // 0xannn is Set I to nnn
                m_specialRegisters->I = static_cast<uint16_t>((word2 << 8) + (word3 << 4) + word4);
                break;
            case 0xb:
                // 0xbnnn is Jump to location nnn + V0.
                m_programCounter = static_cast<uint16_t>(m_registers[0x0] + static_cast<uint16_t>((word2 << 8) + (word3 << 4) + word4));
                break;
            case 0xd:
                // 0xdxyn is Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
                m_registers[0xf] = 0x0;
                for(int i=0; i < word4; ++i)
                {
                    byte = m_memory[m_specialRegisters->I + i];
                    for(int j=0; j < 8; ++j)
                    {
                        x_coord = (m_registers[word2] + 8 - j) % CHIP8_SCREEN_WIDTH;
                        y_coord = (m_registers[word3] + i) % CHIP8_SCREEN_HEIGHT;
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
                
                // 0xfx07 is Set Vx = delay timer value
                if(word3 == 0x0 && word4 == 0x7)
                {
                    m_registers[word2] = m_specialRegisters->DT;
                }
                // 0xfx15 is Set delay timer = Vx
                if(word3 == 0x1 && word4 == 0x5)
                {
                    m_specialRegisters->DT = m_registers[word2];
                }
                // 0xfx18 is Set sound timer = Vx
                if(word3 == 0x1 && word4 == 0x8)
                {
                    m_specialRegisters->ST = m_registers[word2];
                }
                 // 0xfx1e is Set I = I + Vx
                if(word3 == 0x1 && word4 == 0xe)
                {
                    m_specialRegisters->I = static_cast<uint16_t>(m_specialRegisters->I + m_registers[word2]);
                }
                // 0xfx29 is Set I to location of sprite for digit Vx
                if(word3 == 0x2 && word4 == 0x9)
                {
                    m_specialRegisters->I = static_cast<uint16_t>(address_boundaries::RAM_START + m_registers[word2] * CHIP8_HEX_SPRITE_SIZE_BYTES);
                }
                break;
        }

}

void Chip8Interpreter::ExecuteProgram()
{
    uint8_t words[4];
    m_programCounter = address_boundaries::PROGRAM_START;
    m_exit = false;
    m_stackPointer = 0;

    while(!m_exit)
    {
        words[0] = (m_memory[m_programCounter + 0] & 0xf0) >> 4;
        words[1] = (m_memory[m_programCounter + 0] & 0x0f) >> 0;
        words[2] = (m_memory[m_programCounter + 1] & 0xf0) >> 4;
        words[3] = (m_memory[m_programCounter + 1] & 0x0f) >> 0;

        printf("%04x: %01x %01x %01x %01x\n", m_programCounter, words[0], words[1], words[2], words[3]);
        m_programCounter += 2;

        ProcessInstruction(words[0], words[1], words[2], words[3]);
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
        words[0] = (program[i] & 0xff00) >> 8;
        words[1] = (program[i] & 0x00ff) >> 0;


        for(int j=0; j < 2; ++j)
        {
            m_memory[address_boundaries::PROGRAM_START + i * 2 + j] = words[j];
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
