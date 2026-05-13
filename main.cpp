#include <iostream>
#include "allocator.h"
using namespace std;

struct Point {
    int x;
    int y;
};

int main() {
    LinearAllocator allocator(1024);
    
    cout << "=== Linear Allocator Demo ===" << endl;
    cout << "Pool size: 1024 bytes\n" << endl;
    
    // Allocate an int
    cout << "Allocating int (4 bytes)..." << endl;
    int* num = static_cast<int*>(allocator.allocate(sizeof(int)));
    if (num) {
        *num = 42;
        cout << "  Address: " << num << endl;
        cout << "  Value: " << *num << endl;
        cout << "  Offset: " << allocator.get_offset() << endl;
        cout << "  Remaining: " << allocator.get_remaining() << " bytes\n" << endl;
    }
    
    // Allocate an array
    cout << "Allocating int array[5] (20 bytes)..." << endl;
    int* arr = static_cast<int*>(allocator.allocate(sizeof(int) * 5));
    if (arr) {
        for (int i = 0; i < 5; i++) {
            arr[i] = i * 10;
        }
        cout << "  Address: " << arr << endl;
        cout << "  Values: ";
        for (int i = 0; i < 5; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
        cout << "  Offset: " << allocator.get_offset() << endl;
        cout << "  Remaining: " << allocator.get_remaining() << " bytes\n" << endl;
    }
    
    // Allocate a struct
    cout << "Allocating Point struct (8 bytes)..." << endl;
    Point* point = static_cast<Point*>(allocator.allocate(sizeof(Point)));
    if (point) {
        point->x = 100;
        point->y = 200;
        cout << "  Address: " << point << endl;
        cout << "  Value: (" << point->x << ", " << point->y << ")" << endl;
        cout << "  Offset: " << allocator.get_offset() << endl;
        cout << "  Remaining: " << allocator.get_remaining() << " bytes\n" << endl;
    }
    
    // Test out-of-memory
    cout << "Attempting to allocate 2000 bytes (exceeds pool)..." << endl;
    void* large = allocator.allocate(2000);
    if (!large) {
        cout << "  Allocation failed: Out of memory (returned nullptr)\n" << endl;
    }
    
    // Reset allocator
    cout << "Resetting allocator..." << endl;
    allocator.reset();
    cout << "  Offset after reset: " << allocator.get_offset() << endl;
    cout << "  Remaining after reset: " << allocator.get_remaining() << " bytes\n" << endl;
    
    // Allocate after reset
    cout << "Allocating int after reset..." << endl;
    int* new_num = static_cast<int*>(allocator.allocate(sizeof(int)));
    if (new_num) {
        *new_num = 99;
        cout << "  Address: " << new_num << endl;
        cout << "  Value: " << *new_num << endl;
        cout << "  Offset: " << allocator.get_offset() << endl;
    }
    
    return 0;
}
