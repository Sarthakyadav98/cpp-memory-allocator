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
    
    if (aligned_offset + size > pool_size) {
        return nullptr;
    }
    
    void* ptr = memory_pool + aligned_offset;
    offset = aligned_offset + size;
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
