# Phase 2 — Free List Allocator

## Concept
Transform linear allocator into free list allocator by:
1. Adding block metadata (header before each allocation)
2. Maintaining linked list of free blocks
3. Implementing deallocation

## Memory Layout
```
[HEADER|USER DATA][HEADER|USER DATA][HEADER|USER DATA]...
```

## BlockHeader Structure
```cpp
struct BlockHeader {
    size_t size;        // Size of user memory
    bool is_free;       // Is this block free?
    BlockHeader* next;  // Pointer to next free block
};
```

## Free List
Linked list tracking all freed blocks:
```
free_list -> [Free Block] -> [Free Block] -> [Free Block] -> nullptr
```

## Implementation

**allocate(size)**
1. Search free list for suitable block (first-fit)
2. If found: mark as used, remove from free list, return
3. If not found: allocate new block from pool

**free(ptr)**
1. Get header from user pointer (ptr - sizeof(BlockHeader))
2. Mark block as free
3. Add to front of free list

**reset()**
- Clears offset and free list

## Benefits
- Memory reuse: freed blocks can be reallocated
- No memory waste from abandoned allocations
- Foundation for more advanced allocators (best-fit, buddy system)

## Next Steps
- Add block coalescing (merge adjacent free blocks)
- Implement best-fit or worst-fit strategies
- Add fragmentation metrics
