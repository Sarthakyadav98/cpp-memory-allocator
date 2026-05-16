# Custom Memory Allocator with Performance Benchmarking — Interview Revision Guide

## Project Overview

### Project Title

Custom Memory Allocator with Performance Benchmarking

### Tech Stack

* C++
* Linux
* STL
* High-resolution benchmarking using chrono

---

# Resume Description

## Resume Bullet Points

* Implemented a custom memory allocator using free-list management, block splitting, and coalescing strategies.
* Evaluated allocator performance against standard malloc/free under high-allocation workloads, analyzing fragmentation and allocation latency.
* Optimized allocation efficiency by reducing fragmentation and improving memory reuse patterns.

---

# 1. Why This Project?

## Core Motivation

Standard allocators like malloc/free are general-purpose allocators designed to work efficiently across a broad range of workloads.

However, many systems applications have predictable allocation patterns:

* game engines
* simulations
* temporary frame allocators
* compilers
* rendering pipelines
* ML runtimes

Custom allocators can optimize specifically for such workloads.

---

# 2. Core Concepts Used

## Concepts Covered

### Memory Management

* heap memory
* stack vs heap
* pointer arithmetic
* alignment
* fragmentation
* dynamic allocation

### C++ Concepts

* RAII
* constructors/destructors
* raw pointers
* void pointers
* smart memory ownership concepts

### Systems Concepts

* cache locality
* allocation overhead
* memory reuse
* performance tradeoffs

---

# 3. Allocator Evolution

# Phase 1 — Linear/Bump Allocator

## Idea

Allocator maintains:

* a fixed memory pool
* a current offset

Allocations happen sequentially.

## Memory Layout

```text
[USED][USED][USED][FREE FREE FREE...]
```

## Allocation Strategy

```cpp
void* ptr = memory_pool + offset;
offset += size;
```

## Complexity

### Allocation

O(1)

### Reset

O(1)

---

# 4. Why Use char* Memory Pool?

```cpp
memory_pool = new char[size];
```

## Reason

char is 1 byte.

This enables:

* byte-level memory management
* precise pointer arithmetic
* raw memory handling

---

# 5. Alignment Handling

## Problem

Improperly aligned memory can:

* reduce CPU efficiency
* cause undefined behavior on some architectures

## Solution

Implemented 8-byte alignment.

```cpp
size_t align_forward(size_t address, size_t alignment)
```

## Why 8-byte Alignment?

Many important types require 8-byte alignment:

* double
* long long
* pointers on 64-bit systems

---

# 6. Free List Management

# Why Needed?

Linear allocator cannot reuse freed memory.

Without reuse:

* memory exhaustion occurs quickly
* fragmentation increases

## Solution

Implemented free-list based allocation.

## Free List Structure

Each free block stores metadata:

```text
[HEADER][FREE MEMORY]
```

Header stores:

* block size
* free status
* next free block pointer

---

# 7. Block Splitting

## Problem

Suppose:

* free block = 100 bytes
* requested = 40 bytes

Using entire 100 bytes wastes memory.

## Solution

Split block.

```text
Before:
[ FREE 100 ]

After:
[ USED 40 ][ FREE 60 ]
```

## Benefit

Reduces internal fragmentation.
Improves memory utilization.

---

# 8. Coalescing

## Problem

Repeated allocations/deallocations create fragmented free memory.

Example:

```text
[ FREE 40 ][ FREE 60 ]
```

## Solution

Merge adjacent free blocks.

```text
[ FREE 100 ]
```

## Benefit

Reduces external fragmentation.
Improves ability to satisfy larger allocations.

---

# 9. Fragmentation Types

## Internal Fragmentation

Unused memory inside allocated block.

Example:

* request = 18 bytes
* allocated = 24 bytes

6 bytes wasted internally.

## External Fragmentation

Enough free memory exists overall but scattered in small non-contiguous chunks.

Example:

```text
[FREE 10][USED][FREE 10]
```

Cannot allocate 15 contiguous bytes.

---

# 10. Benchmarking Strategy

Benchmarked allocator against malloc/free.

Used:

* high allocation workloads
* sequential allocations
* allocation/deallocation reuse patterns
* bulk reset behavior

Metrics measured:

* allocation latency
* throughput
* deallocation overhead
* memory reuse efficiency

---

# 11. Benchmark Results

# Benchmark 1 — Small Allocations

### Workload

100,000 allocations × 16 bytes

### Results

* Custom Allocator: 2,589 μs
* malloc: 5,434 μs
* ~2.1x faster

## Why Faster?

* minimal bookkeeping
* no thread synchronization
* simple sequential allocation
* lower overhead

---

# Benchmark 2 — Allocation/Deallocation Reuse

### Results

malloc/free performed better.

## Why malloc Won?

* decades of optimization
* sophisticated reuse strategies
* highly optimized free-list management
* hardware-level tuning

## Important Insight

General-purpose allocators outperform custom allocators in many complex workloads.

---

# Benchmark 3 — Sequential Allocations

### Workload

200,000 allocations × 8 bytes

### Results

* Custom Allocator: 2,544 μs
* malloc: 9,016 μs
* ~3.5x faster

## Why Faster?

* contiguous memory access
* cache-friendly allocation pattern
* offset-based bump allocation
* minimal metadata overhead

---

# Benchmark 4 — Bulk Deallocation

### Results

Custom reset() dramatically faster.

## Why?

### reset()

O(1)

Only:

```cpp
offset = 0;
```

### free()

Requires:

* per-block deallocation
* metadata handling
* allocator bookkeeping

---

# 12. Cache Locality

## Why Contiguous Memory Matters

CPU caches nearby memory together.

Sequential allocations improve:

* cache hits
* iteration performance
* memory access efficiency

This is one reason vector and bump allocators are performant.

---

# 13. Time Complexities

| Operation        | Complexity      |
| ---------------- | --------------- |
| Linear allocate  | O(1)            |
| reset()          | O(1)            |
| Free-list search | O(n) worst case |
| Block splitting  | O(1)            |
| Coalescing       | O(n)            |

---

# 14. Important Tradeoffs

## Advantages

* very fast sequential allocations
* low overhead
* excellent cache locality
* efficient bulk deallocation
* specialized workload optimization

## Limitations

* not thread-safe
* not ideal for unpredictable workloads
* malloc/free more optimized generally
* fragmentation management complexity

---

# 15. Real-World Use Cases

## Where This Allocator Performs Well

### Game Engines

Per-frame allocations with bulk reset.

### Temporary Buffers

Scratch memory for computations.

### Compilers

AST/node allocations.

### Simulations

Large sequential memory workloads.

### Single-threaded Systems

Avoid synchronization overhead.

---

# 16. Important Interview Questions

# Q1 — Why build a custom allocator?

General-purpose allocators optimize for broad workloads.
Custom allocators optimize specific allocation patterns to reduce overhead and improve memory locality.

---

# Q2 — Why was your allocator faster?

The allocator reduced bookkeeping overhead and optimized sequential contiguous allocations with better cache locality.

---

# Q3 — Why did malloc outperform in some benchmarks?

malloc/free are highly optimized general-purpose allocators with advanced reuse and fragmentation handling strategies.

---

# Q4 — What is fragmentation?

Fragmentation is inefficient memory utilization caused by allocation/deallocation patterns.

* internal fragmentation
* external fragmentation

---

# Q5 — How did you reduce fragmentation?

Implemented:

* block splitting
* coalescing
* free-list reuse

---

# Q6 — Why alignment matters?

Proper alignment improves CPU memory access efficiency and avoids potential undefined behavior.

---

# Q7 — Why is reset() so fast?

Because reset only updates allocator state by resetting offset instead of individually freeing blocks.

---

# Q8 — Why use char* memory pool?

char provides byte-level granularity for raw memory manipulation and pointer arithmetic.

---

# Q9 — What is external fragmentation?

Free memory exists overall but is split into non-contiguous chunks.

---

# Q10 — What is coalescing?

Merging adjacent free blocks to reduce fragmentation and improve memory reuse.

---

# 17. Key Systems Concepts Demonstrated

This project demonstrates understanding of:

* heap memory management
* allocator design
* pointer arithmetic
* alignment handling
* fragmentation management
* cache locality
* benchmarking methodology
* memory reuse strategies
* low-level systems optimization

---

# 18. Final Conclusion

This project explored the design of specialized memory allocators optimized for predictable allocation workloads.

The allocator demonstrated strong performance in:

* sequential allocation patterns
* bulk deallocation scenarios
* cache-friendly workloads

while also highlighting the complexity and engineering sophistication of modern general-purpose allocators like malloc/free.

The project provided hands-on experience with:

* low-level m
