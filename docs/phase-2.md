# Phase 2 — Alignment Support

## Problem
Without alignment, allocations can be misaligned, causing:
- Slower memory access
- Undefined behavior on some architectures
- Potential crashes on strict alignment platforms

## Example Issue
```cpp
allocate(1);           // Returns address 0
allocate(sizeof(int)); // Returns address 1 (misaligned!)
```

An `int` at address 1 is not 4-byte aligned, which can cause performance issues or crashes.

## Solution: 8-Byte Alignment
All allocations are aligned to 8-byte boundaries.

### Implementation
1. **ALIGNMENT constant**: Set to 8 bytes
2. **align_forward()**: Helper function that rounds up addresses to alignment boundary
3. **Modified allocate()**: Aligns offset before returning pointer

### How It Works
```
Before alignment:
[X][_][_][_][_][_][_][_]  <- allocate(1) returns offset 0
 ^                           next offset = 1 (misaligned!)

After alignment:
[X][_][_][_][_][_][_][_][Y][Y][Y][Y]...
 ^                       ^
 offset 0                offset 8 (aligned!)
```

### Benefits
- All pointers are properly aligned
- Safe for any data type (int, double, structs)
- Better performance on modern CPUs
- Portable across architectures
