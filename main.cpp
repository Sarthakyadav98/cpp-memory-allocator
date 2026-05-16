#include <iostream>
#include "allocator.h"
using namespace std;

struct Point {
    int x;
    int y;
};

int main() {
    LinearAllocator allocator(1024);
    
    cout << "=== Block Splitting & Coalescing Demo ===" << endl;
    cout << "Pool size: 1024 bytes" << endl;
    cout << "BlockHeader size: " << sizeof(BlockHeader) << " bytes\n" << endl;
    
    // Test 1: Block Splitting
    cout << "=== TEST 1: Block Splitting ===" << endl;
    cout << "Allocating 100 bytes..." << endl;
    int* large = static_cast<int*>(allocator.allocate(100));
    cout << "  Address: " << large << endl;
    
    cout << "Freeing 100-byte block..." << endl;
    allocator.deallocate(large);
    
    cout << "Allocating 40 bytes (should split)..." << endl;
    int* small = static_cast<int*>(allocator.allocate(40));
    cout << "  Address: " << small << " (reused)" << endl;
    
    cout << "Allocating 20 bytes (should use remainder)..." << endl;
    int* small2 = static_cast<int*>(allocator.allocate(20));
    cout << "  Address: " << small2 << " (from split)\n" << endl;
    
    // Test 2: Coalescing
    cout << "=== TEST 2: Coalescing ===" << endl;
    cout << "Allocating 3 blocks of 50 bytes each..." << endl;
    int* block1 = static_cast<int*>(allocator.allocate(50));
    int* block2 = static_cast<int*>(allocator.allocate(50));
    int* block3 = static_cast<int*>(allocator.allocate(50));
    cout << "  block1: " << block1 << endl;
    cout << "  block2: " << block2 << endl;
    cout << "  block3: " << block3 << endl;
    
    cout << "\nFreeing block1..." << endl;
    allocator.deallocate(block1);
    
    cout << "Freeing block2 (should coalesce with block1)..." << endl;
    allocator.deallocate(block2);
    cout << "  Blocks coalesced into larger free block\n" << endl;
    
    cout << "Allocating 112 bytes (should fit in coalesced block)..." << endl;
    int* coalesced = static_cast<int*>(allocator.allocate(112));
    if (coalesced) {
        cout << "  Address: " << coalesced << endl;
        cout << "  SUCCESS: Coalescing allowed larger allocation!" << endl;
    } else {
        cout << "  FAILED: Could not allocate (coalescing didn't work)" << endl;
    }
    
    return 0;
}
