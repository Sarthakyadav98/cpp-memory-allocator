#include "allocator.h"
#include <new>

LinearAllocator::LinearAllocator(size_t size) 
    : pool_size(size), offset(0), free_list(nullptr) {
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
    // First, try to find a free block
    BlockHeader* prev = nullptr;
    BlockHeader* current = free_list;
    
    while (current != nullptr) {
        if (current->is_free && current->size >= size) {
            // Found a suitable free block
            current->is_free = false;
            
            // Remove from free list
            if (prev == nullptr) {
                free_list = current->next;
            } else {
                prev->next = current->next;
            }
            
            return reinterpret_cast<char*>(current) + sizeof(BlockHeader);
        }
        prev = current;
        current = current->next;
    }
    
    // No free block found, allocate new
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


void LinearAllocator::free(void* ptr) {
    if (ptr == nullptr) {
        return;
    }
    
    // Get header from user pointer
    BlockHeader* header = reinterpret_cast<BlockHeader*>(
        reinterpret_cast<char*>(ptr) - sizeof(BlockHeader)
    );
    
    // Mark as free
    header->is_free = true;
    
    // Add to front of free list
    header->next = free_list;
    free_list = header;
}

void LinearAllocator::reset() {
    offset = 0;
    free_list = nullptr;
}

size_t LinearAllocator::get_offset() const {
    return offset;
}

size_t LinearAllocator::get_remaining() const {
    return pool_size - offset;
}
