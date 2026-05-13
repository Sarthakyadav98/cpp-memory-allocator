#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>

class LinearAllocator {
private:
    char* memory_pool;
    size_t pool_size;
    size_t offset;

public:
    LinearAllocator(size_t size);
    ~LinearAllocator();
    
    void* allocate(size_t size);
    void reset();
    
    size_t get_offset() const;
    size_t get_remaining() const;
};

#endif
