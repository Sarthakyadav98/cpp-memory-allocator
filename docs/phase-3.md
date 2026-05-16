# Phase 3 — Block Splitting

## Problem
Without splitting, when we reuse a large free block for a small allocation, we waste the remaining space.

**Example:**
- Free block: 100 bytes
- Request: 40 bytes
- Without splitting: Entire 100 bytes marked as used (60 bytes wasted)

## Solution: Block Splitting
When a free block is larger than needed, split it into two blocks:
1. **Used block** - exact size requested
2. **Free block** - remaining space

## Visualization
```
Before allocation:
[HEADER|FREE 100 bytes]

After allocate(40):
[HEADER|USED 40][HEADER|FREE 48]
                 ^new header created
```

## Implementation Logic
```cpp
if (free_block.size >= requested_size) {
    remaining = free_block.size - requested_size;
    
    if (remaining >= min_split_size) {
        // Split: create new free block from remainder
        // Keep remainder in free list
    } else {
        // Too small to split, use entire block
    }
}
```

## Minimum Split Size
Don't split if remainder is too small:
```
min_split_size = sizeof(BlockHeader) + ALIGNMENT
```

This prevents creating tiny unusable blocks.

## Benefits
- **Reduced waste**: Unused portions stay available
- **Better memory utilization**: Small allocations don't consume large blocks
- **Improved fragmentation**: More granular free blocks

## Memory Layout Example
```
Allocate 100 bytes, then free it, then allocate 40:

[HEADER|USED 40][HEADER|FREE 48][other blocks...]
     ^used            ^available for reuse
```
