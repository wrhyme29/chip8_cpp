#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>

#define CHIP8_RAM_SIZE_BYTES 4096
#define CHIP8_HEX_SPRITE_SIZE_BYTES 5
#define CHIP8_SPRITE_WIDTH_PX 8
#define CHIP8_STACK_ADDRESS_COUNT 16
#define CHIP8_REGISTERS_COUNT 16
#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32

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
    void ProcessInstruction(uint8_t word1, uint8_t word2, uint8_t word3, uint8_t word4);
    uint8_t* GetHexSprite(uint8_t hexChar);
    void LoadProgramIntoMemory(const uint16_t* program, uint16_t size);
    uint8_t GetRegistryValue(uint8_t registerName);
    bool** GetScreen(void);
    void SetRegisterI(uint16_t memoryAddress);
    void SetRenderer(std::function<void(bool**)> renderCallback);

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
    bool m_exit;
    uint16_t* m_stack;
    uint16_t m_programCounter;
    uint8_t m_stackPointer;
    bool** m_screen;
    std::function<void(bool**)> m_renderCallback;
};
