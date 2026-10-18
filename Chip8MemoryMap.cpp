#include <memory>

// Following RAII, allocate the buffer on construction
Chip8MemoryMap::Chip8MemoryMap() : 
    m_size(4096)
{
    m_buffer = new char[m_size];
}

// Following RAII, deallocate the buffer on destruction
Chip8MemoryMap::~Chip8MemoryMap()
{
    delete[] m_buffer;
}
