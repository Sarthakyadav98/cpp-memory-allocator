# Phase 1 — Linear/Bump Allocator ✓

## Implemented Features

**Core Allocator**
- Fixed-size memory pool (dynamically allocated)
- Offset tracking for sequential allocations
- `allocate(size)` - returns aligned pointer or nullptr
- `reset()` - clears all allocations instantly
- Out-of-memory handling

**8-Byte Alignment**
- All allocations aligned to 8-byte boundaries
- `align_forward()` helper for address alignment
- Prevents misalignment issues and undefined behavior

**Testing**
- Allocates int, arrays, and structs
- Demonstrates contiguous allocation
- Tests OOM handling and reset functionality

## Files
- `allocator.h` - LinearAllocator class definition
- `allocator.cpp` - Implementation with alignment
- `main.cpp` - Comprehensive demo
