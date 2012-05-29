#include <malloc.h>
#include "BlockAllocator.h"

struct Block
{
	size_t m_size;
	size_t m_used;
	char* m_buffer;
	Block* m_next;
};

/* Private API */
void BlockAllocator_Free(BlockAllocator* const pThis)
{
	BlockAllocator temp;
	temp.m_blocksize = pThis->m_blocksize;
	temp.m_head = pThis->m_head;

	pThis->m_blocksize = 0;
	pThis->m_head = NULL;
	BlockAllocator_Destroy(&temp);
}

/* Public API */
void BlockAllocator_Init(BlockAllocator* const pThis, const size_t blocksize)
{
	pThis->m_head = NULL;
	pThis->m_blocksize = blocksize;
}

void BlockAllocator_Destroy(BlockAllocator* const pThis)
{
	while (pThis->m_head)
	{
		Block* const temp = pThis->m_head->m_next;
		free(pThis->m_head);
		pThis->m_head = temp;
	}
}

void* BlockAllocator_Malloc(BlockAllocator* const pThis, const size_t size)
{
	void* ptr;
	if ((pThis->m_head && pThis->m_head->m_used + size > pThis->m_head->m_size) || !pThis->m_head)
	{
		/* calc needed size for allocation */
		size_t alloc_size = ((sizeof(Block) + size) > pThis->m_blocksize) ? (sizeof(Block) + size) : pThis->m_blocksize;

		/* create new block */
		char* buffer = (char*)malloc(alloc_size);
		Block* b = (Block*)(buffer);
		b->m_size = alloc_size;
		b->m_used = sizeof(Block);
		b->m_buffer = buffer;
		b->m_next = pThis->m_head;
		pThis->m_head = b;
	}

	ptr = pThis->m_head->m_buffer + pThis->m_head->m_used;
	pThis->m_head->m_used += size;
	return ptr;
}

