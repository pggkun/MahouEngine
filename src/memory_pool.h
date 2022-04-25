#ifndef _MEMORY_POOL_
#define _MEMORY_POOL_

#include <cstdint>

using uint = unsigned int;

class MemoryPool
{
private:
    const uint headerSize = sizeof(uint8_t *);
    uint blockCount;
    uint blockSize;
    uint8_t *memory;
    uint8_t *head;

public:
    MemoryPool(uint dataCount, uint dataSize);
    ~MemoryPool();

    void *New();
    void Delete(void *mem);
};

#endif