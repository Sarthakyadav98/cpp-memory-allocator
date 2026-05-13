#include "allocator.h"
#include <new>

LinearAllocator::LinearAllocator(size_t size) 
    : pool_size(size), offset(0) {
    memory_pool = new char[size];
}

LinearAllocator::~LinearAllocator() {
    delete[] memory_pool;
}

void* LinearAllocator::allocate(size_t size) {
    if (offset + size > pool_size) {
        return nullptr;
    }
    
    void* ptr = memory_pool + offset;
    offset += size;
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
