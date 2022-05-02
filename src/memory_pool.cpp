#include "memory_pool.h"
#include <iostream>

using uint = unsigned int;

MemoryPool::MemoryPool(uint dataCount, uint dataSize) : blockCount(dataCount),
                                              blockSize(headerSize + dataSize),
                                              memory(new uint8_t[blockCount * blockSize]),
                                              head(memory)
{
    uint8_t **pointer;
    uint8_t *pCurr = head;
    for (uint i = 0; i < blockCount - 1; ++i)
    {
        uint8_t *pNext = pCurr + blockSize;
        pointer = reinterpret_cast<uint8_t **>(pCurr);
        *pointer = pNext;
        pCurr = pNext;
    }

    pointer = reinterpret_cast<uint8_t **>(pCurr);
    *pointer = nullptr;
}

MemoryPool::~MemoryPool()
{
    delete[] memory;
}

void *MemoryPool::New()
{
    if (!head)
        return nullptr;

    uint8_t *dataPtr = head + headerSize;
    uint8_t **pointer = reinterpret_cast<uint8_t **>(head);
    head = *pointer;
    return dataPtr;
}

void MemoryPool::Delete(void *mem)
{
    if (!mem)
        return;

    uint8_t *block = static_cast<uint8_t *>(mem) - headerSize;
    uint8_t **pointer = reinterpret_cast<uint8_t **>(block);
    *pointer = head;
    head = block;
}