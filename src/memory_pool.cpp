#include "memory_pool.h"
#include <iostream>

using uint = unsigned int;

MemoryPool::MemoryPool(uint dataCount, uint dataSize) : blockCount(dataCount),                    // quantidade de blocos
                                              blockSize(headerSize + dataSize),         // tamanho do bloco = cabeçalho + dado
                                              memory(new uint8_t[blockCount * blockSize]), // memória para armazenar dados
                                              head(memory)                              // local da próxima alocação
{
    // liga ponteiros dos cabeçalhos
    uint8_t **pointer;
    uint8_t *pCurr = head;
    for (uint i = 0; i < blockCount - 1; ++i)
    {
        // calcula a posição do próximo bloco
        uint8_t *pNext = pCurr + blockSize;

        // pointer permite alterar 4 uint8_ts da memória a partir da posição pCurr
        pointer = reinterpret_cast<uint8_t **>(pCurr);
        *pointer = pNext;

        // avança para próximo bloco
        pCurr = pNext;
    }

    // último cabeçalho aponta para nulo
    pointer = reinterpret_cast<uint8_t **>(pCurr);
    *pointer = nullptr;
}

MemoryPool::~MemoryPool()
{
    delete[] memory;
}

void *MemoryPool::New()
{
    // se não existe memória disponível retorne nulo
    if (!head)
        return nullptr;

    // pegue endereço do bloco de dados
    uint8_t *dataPtr = head + headerSize;

    // cabeça da lista passa para próximo bloco disponível
    uint8_t **pointer = reinterpret_cast<uint8_t **>(head);
    head = *pointer;

    // retorna endereço de um bloco de dados
    return dataPtr;
}

void MemoryPool::Delete(void *mem)
{
    // se endereço é nulo
    if (!mem)
        return;

    // retorna para o endereço do bloco inteiro (incluindo cabeçalho)
    uint8_t *block = static_cast<uint8_t *>(mem) - headerSize;

    // muda ponteiro do cabeçalho para apontar para cabeça da lista
    uint8_t **pointer = reinterpret_cast<uint8_t **>(block);
    *pointer = head;

    // nova cabeça da lista
    head = block;
}