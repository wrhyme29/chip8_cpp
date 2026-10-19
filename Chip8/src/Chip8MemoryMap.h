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
    
    explicit Chip8MemoryMap();
    ~Chip8MemoryMap();

    // Disallow copying to prevent multiple Chip8MemoryMaps 
    // from managing the same buffer
    Chip8MemoryMap(const Chip8MemoryMap&) = delete;
    Chip8MemoryMap& operator=(const Chip8MemoryMap&) = delete;
private:
    uint8_t* m_buffer;
    size_t m_size;
};
