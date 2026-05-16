#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>

struct BlockHeader {
    size_t size;
    bool is_free;
    BlockHeader* next;
};

class LinearAllocator {
private:
    static constexpr size_t ALIGNMENT = 8;
    
    char* memory_pool;
    size_t pool_size;
    size_t offset;
    BlockHeader* free_list;
    
    size_t align_forward(size_t address, size_t alignment);

public:
    LinearAllocator(size_t size);
    ~LinearAllocator();
    
    void* allocate(size_t size);
    void free(void* ptr);
    void reset();
    
    size_t get_offset() const;
    size_t get_remaining() const;
};

#endif
