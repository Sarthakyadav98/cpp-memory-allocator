#include "allocator.h"
#include <new>

LinearAllocator::LinearAllocator(size_t size) 
    : pool_size(size), offset(0) {
    memory_pool = new char[size];
}

LinearAllocator::~LinearAllocator() {
    delete[] memory_pool;
}

size_t LinearAllocator::align_forward(size_t address, size_t alignment) {
    size_t modulo = address % alignment;
    if (modulo != 0) {
        address += (alignment - modulo);
    }
    return address;
}

void* LinearAllocator::allocate(size_t size) {
    size_t aligned_offset = align_forward(offset, ALIGNMENT);
    size_t total_size = sizeof(BlockHeader) + size;
    
    if (aligned_offset + total_size > pool_size) {
        return nullptr;
    }
    
    // Create header
    BlockHeader* header = reinterpret_cast<BlockHeader*>(memory_pool + aligned_offset);
    header->size = size;
    header->is_free = false;
    header->next = nullptr;
    
    // Return pointer to user memory (after header)
    void* ptr = memory_pool + aligned_offset + sizeof(BlockHeader);
    offset = aligned_offset + total_size;
    return ptr;
}

void LinearAllocator::reset() {
    offset = 0;
}

size_t LinearAllocator::get_offset() const {
    return offset;
}

size_t LinearAllocator::get_remaining() const {
    return pool_size - offset;
}
