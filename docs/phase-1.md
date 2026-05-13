V1 — Linear / Bump Allocator

REQUIRED FEATURES
1. Fixed Memory Pool

Create a fixed-size memory buffer.

Example:

char memory_pool[1024];

This acts like your own mini heap.

2. Offset Tracker

Keep track of:

how much memory already used

Example:

size_t offset = 0;
3. allocate(size)

Function:

void* allocate(size_t size)

Behavior:

check remaining memory
return current free address
move offset forward
4. Out-of-Memory Handling

If request exceeds pool:

return nullptr
5. reset()

Function:

void reset()

Resets allocator completely.

Like:

free all allocations at once
6. Testing

Allocate:

int
arrays
structs

Print:

addresses
offsets

Observe contiguous allocation.