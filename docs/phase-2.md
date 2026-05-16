# Phase 2 — Block Metadata (Foundation for Free List)

## Concept
Each allocation now has metadata stored before the user memory:

```
[HEADER][USER MEMORY]
```

## BlockHeader Structure
```cpp
struct BlockHeader {
    size_t size;        // Size of user memory
    bool is_free;       // Is this block free?
    BlockHeader* next;  // Pointer to next block
};
```

## Memory Layout
```
Before (Linear only):
[USER DATA][USER DATA][USER DATA]...

After (With headers):
[HEADER|USER DATA][HEADER|USER DATA][HEADER|USER DATA]...
```

## Changes
- `allocate()` now creates a header before returning user pointer
- Total allocation = `sizeof(BlockHeader) + requested_size`
- User gets pointer to memory after the header
- Header tracks size, free status, and next block

## Next Steps
This prepares for:
- `free()` function (mark blocks as free)
- Free list traversal (using next pointers)
- Block reuse (find and reuse free blocks)
