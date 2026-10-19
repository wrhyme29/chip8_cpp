#include <cstddef>
#include <cstdint>

class Chip8MemoryMap
{
public:
    enum address_boundaries
    {
        RAM_START = 0x000, 
        PROGRAM_START = 0x200,
        ETI_PROGRAM_START = 0x600,
        RAM_END = 0xFFF
    };

    struct Registers
    {
        uint8_t  V1;
        uint8_t  V2;
        uint8_t  V3;
        uint8_t  V4;
        uint8_t  V5;
        uint8_t  V6;
        uint8_t  V7;
        uint8_t  V8;
        uint8_t  V9;
        uint8_t  VA;
        uint8_t  VB;
        uint8_t  VC;
        uint8_t  VD;
        uint8_t  VE;
        uint8_t  VF;
        uint16_t I;
        uint8_t  DT;
        uint8_t  ST;
    };
    
    explicit Chip8MemoryMap();
    ~Chip8MemoryMap();

    // Disallow copying to prevent multiple Chip8MemoryMaps 
    // from managing the same buffer
    Chip8MemoryMap(const Chip8MemoryMap&) = delete;
    Chip8MemoryMap& operator=(const Chip8MemoryMap&) = delete;

    uint8_t* GetHexSprite(uint8_t hexChar);

    Registers m_registers;

private:
    const uint8_t c_hexSprites[5 * 16] = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7    
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B   
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    uint8_t* m_buffer;
    size_t m_size;

    void LoadHexSpritesIntoMemory(void);
};
