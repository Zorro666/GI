#ifndef BLOCK_ALLOCATOR_H
#define BLOCK_ALLOCATOR_H

typedef struct block_allocator block_allocator;

void block_allocator_init(block_allocator* const pThis, size_t blocksize);
void block_allocator_destroy(block_allocator* const pThis);

void* block_allocator_malloc(block_allocator* const pThis, size_t size);

#endif
