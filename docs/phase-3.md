# Phase 3 — Block Splitting & Coalescing

## Part 1: Block Splitting

### Problem
Without splitting, when we reuse a large free block for a small allocation, we waste the remaining space.

**Example:**
- Free block: 100 bytes
- Request: 40 bytes
- Without splitting: Entire 100 bytes marked as used (60 bytes wasted)

### Solution: Block Splitting
When a free block is larger than needed, split it into two blocks:
1. **Used block** - exact size requested
2. **Free block** - remaining space

### Visualization
```
Before allocation:
[HEADER|FREE 100 bytes]

After allocate(40):
[HEADER|USED 40][HEADER|FREE 48]
                 ^new header created
```

### Minimum Split Size
```
min_split_size = sizeof(BlockHeader) + ALIGNMENT
```

Use `>=` not `>` because even a 0-byte block is useful for future coalescing.

---

## Part 2: Coalescing

### Problem
After multiple allocations and deallocations, you get fragmentation:
```
[FREE 40][FREE 60][FREE 30]
```
These small blocks can't satisfy a 100-byte request, even though total free = 130 bytes.

### Solution: Coalescing
When freeing a block, merge it with adjacent free blocks:
```
Before: [FREE 40][FREE 60]
After:  [FREE 100]
```

### Implementation
When `deallocate()` is called:
1. Mark block as free
2. Check if next adjacent block is free
3. If yes, merge: `new_size = size1 + sizeof(BlockHeader) + size2`
4. Remove merged block from free list
5. Add coalesced block to free list

### Memory Layout
```
Before coalescing:
[HEADER|FREE 40][HEADER|FREE 60]

After coalescing:
[HEADER|FREE 112]
(40 + 12 + 60 = 112)
```

### Benefits
- **Reduces external fragmentation**
- **Larger contiguous blocks available**
- **Better memory utilization**
- **Can satisfy larger allocation requests**

### Limitations (Current Implementation)
- Only forward coalescing (with next block)
- Doesn't coalesce with previous block
- For full coalescing, need doubly-linked list or sorted free list
