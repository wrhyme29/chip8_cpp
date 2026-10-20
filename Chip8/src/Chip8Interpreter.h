#include <cstddef>
#include <cstdint>
#include <memory>

#define CHIP8_RAM_SIZE_BYTES 4096
#define CHIP8_HEX_SPRITE_SIZE_BYTES 5
#define CHIP8_STACK_ADDRESS_COUNT 16
#define CHIP8_REGISTERS_COUNT 16

class Chip8Interpreter
{
public:
    enum address_boundaries : uint16_t
    {
        RAM_START = 0x000, 
        PROGRAM_START = 0x200,
        ETI_PROGRAM_START = 0x600,
        VARS_DISPLAY_REFRESH = 0xE90,
        RAM_END = 0xFFF
    };

    struct SpecialRegisters
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
    
    explicit Chip8Interpreter();
    ~Chip8Interpreter();

    // Disallow copying to prevent multiple Chip8MemoryMaps 
    // from managing the same buffer
    Chip8Interpreter(const Chip8Interpreter&) = delete;
    Chip8Interpreter& operator=(const Chip8Interpreter&) = delete;

    void ExecuteProgram(void);
    uint8_t* GetHexSprite(uint8_t hexChar);
    void LoadProgramIntoMemory(const uint16_t* program, uint16_t size);
    uint8_t GetRegistryValue(uint8_t registerName);
    void SetRegisterI(uint16_t memoryAddress);

private:
    void LoadProgramIntoMemory(const uint8_t* program, uint16_t size, uint16_t startMemoryAddress);


    const uint8_t c_hexSprites[CHIP8_HEX_SPRITE_SIZE_BYTES * 16] = 
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
    uint8_t* m_memory;
    uint8_t* m_registers;
    std::unique_ptr<SpecialRegisters> m_specialRegisters;
    uint16_t* m_stack;


};
