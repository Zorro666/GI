#include <malloc.h>
#include "block_allocator.h"

struct block
{
	size_t size;
	size_t used;
	char* buffer;
	block* next;
};

/* Private API */
void block_allocator_free(block_allocator* const pThis)
{
	block_allocator temp;
	temp.m_blocksize = pThis->m_blocksize;
	temp.m_head = pThis->m_head;

	pThis->m_blocksize = 0;
	pThis->m_head = NULL;
	block_allocator_destroy(&temp);
}

/* Public API */
void block_allocator_init(block_allocator* const pThis, size_t blocksize)
{
	pThis->m_head = NULL;
	pThis->m_blocksize = blocksize;
}

void block_allocator_destroy(block_allocator* const pThis)
{
	while (pThis->m_head)
	{
		block* temp = pThis->m_head->next;
		free(pThis->m_head);
		pThis->m_head = temp;
	}
}

void* block_allocator_malloc(block_allocator* const pThis, size_t size)
{
	void* ptr;
	if ((pThis->m_head && pThis->m_head->used + size > pThis->m_head->size) || !pThis->m_head)
	{
		/* calc needed size for allocation */
		size_t alloc_size = ((sizeof(block) + size) > pThis->m_blocksize) ? (sizeof(block) + size) : pThis->m_blocksize;

		/* create new block */
		char* buffer = (char*)malloc(alloc_size);
		block* b = (block*)(buffer);
		b->size = alloc_size;
		b->used = sizeof(block);
		b->buffer = buffer;
		b->next = pThis->m_head;
		pThis->m_head = b;
	}

	ptr = pThis->m_head->buffer + pThis->m_head->used;
	pThis->m_head->used += size;
	return ptr;
}

