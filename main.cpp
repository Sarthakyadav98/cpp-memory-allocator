#include <iostream>
#include "allocator.h"
using namespace std;

struct Point {
    int x;
    int y;
};

int main() {
    LinearAllocator allocator(1024);
    
    cout << "=== Free List Allocator with Block Splitting ===" << endl;
    cout << "Pool size: 1024 bytes" << endl;
    cout << "BlockHeader size: " << sizeof(BlockHeader) << " bytes\n" << endl;
    
    // Allocate a large block
    cout << "Allocating 100 bytes..." << endl;
    int* large = static_cast<int*>(allocator.allocate(100));
    if (large) {
        cout << "  Address: " << large << endl;
        cout << "  Offset: " << allocator.get_offset() << " bytes\n" << endl;
    }
    
    // Free it
    cout << "Freeing 100-byte block..." << endl;
    allocator.deallocate(large);
    cout << "  Block added to free list\n" << endl;
    
    // Allocate smaller block - should split
    cout << "Allocating 40 bytes (should split the 100-byte block)..." << endl;
    int* small = static_cast<int*>(allocator.allocate(40));
    if (small) {
        cout << "  Address: " << small << endl;
        cout << "  Same address as large? " << (small == large ? "YES" : "NO") << endl;
        cout << "  Offset unchanged: " << allocator.get_offset() << " bytes" << endl;
        cout << "  Remaining ~48 bytes should be in free list\n" << endl;
    }
    
    // Allocate another small block - should use the split remainder
    cout << "Allocating 20 bytes (should use split remainder)..." << endl;
    int* small2 = static_cast<int*>(allocator.allocate(20));
    if (small2) {
        cout << "  Address: " << small2 << endl;
        cout << "  Offset unchanged: " << allocator.get_offset() << " bytes" << endl;
        cout << "  Used the remainder from split!\n" << endl;
    }
    
    // Demonstrate with multiple allocations
    cout << "\nAllocating 3 integers..." << endl;
    int* num1 = static_cast<int*>(allocator.allocate(sizeof(int)));
    int* num2 = static_cast<int*>(allocator.allocate(sizeof(int)));
    int* num3 = static_cast<int*>(allocator.allocate(sizeof(int)));
    
    if (num1 && num2 && num3) {
        *num1 = 10;
        *num2 = 20;
        *num3 = 30;
        cout << "  num1: " << *num1 << " at " << num1 << endl;
        cout << "  num2: " << *num2 << " at " << num2 << endl;
        cout << "  num3: " << *num3 << " at " << num3 << endl;
    }
    
    return 0;
}
