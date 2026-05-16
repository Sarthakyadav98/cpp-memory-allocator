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
            
            // Check if we should split the block
            size_t remaining = current->size - size;
            size_t min_split_size = sizeof(BlockHeader) + ALIGNMENT;
            
            if (remaining >= min_split_size) {
                // Split the block
                current->size = size;
                
                // Create new free block from remaining space
                BlockHeader* new_block = reinterpret_cast<BlockHeader*>(
                    reinterpret_cast<char*>(current) + sizeof(BlockHeader) + size
                );
                new_block->size = remaining - sizeof(BlockHeader);
                new_block->is_free = true;
                new_block->next = current->next;
                
                // Update current block's next pointer
                current->next = new_block;
                
                // Keep new block in free list (don't remove it)
                if (prev == nullptr) {
                    free_list = new_block;
                } else {
                    prev->next = new_block;
                }
            } else {
                // Block too small to split, use entire block
                // Remove from free list
                if (prev == nullptr) {
                    free_list = current->next;
                } else {
                    prev->next = current->next;
                }
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


void LinearAllocator::deallocate(void* ptr) {
    if (ptr == nullptr) {
        return;
    }
    
    // Get header from user pointer
    BlockHeader* header = reinterpret_cast<BlockHeader*>(
        reinterpret_cast<char*>(ptr) - sizeof(BlockHeader)
    );
    
    // Mark as free
    header->is_free = true;
    
    // Try to coalesce with next block if it's adjacent and free
    BlockHeader* next_block = reinterpret_cast<BlockHeader*>(
        reinterpret_cast<char*>(header) + sizeof(BlockHeader) + header->size
    );
    
    // Check if next block is within pool bounds and is free
    char* next_addr = reinterpret_cast<char*>(next_block);
    if (next_addr < memory_pool + offset && next_block->is_free) {
        // Merge with next block
        header->size += sizeof(BlockHeader) + next_block->size;
        header->next = next_block->next;
        
        // Remove next_block from free list
        BlockHeader* current = free_list;
        BlockHeader* prev = nullptr;
        while (current != nullptr) {
            if (current == next_block) {
                if (prev == nullptr) {
                    free_list = current->next;
                } else {
                    prev->next = current->next;
                }
                break;
            }
            prev = current;
            current = current->next;
        }
    }
    
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
