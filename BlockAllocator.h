#ifndef BLOCK_ALLOCATOR_H
#define BLOCK_ALLOCATOR_H

#include <stdlib.h>

typedef struct BlockAllocator BlockAllocator;
typedef struct Block Block;

struct BlockAllocator
{
	Block* m_head;
	size_t m_blocksize;
};

void BlockAllocator_Init(BlockAllocator* const pThis, const size_t blocksize);
void BlockAllocator_Destroy(BlockAllocator* const pThis);

void* BlockAllocator_Malloc(BlockAllocator* const pThis, const size_t size);

#endif
