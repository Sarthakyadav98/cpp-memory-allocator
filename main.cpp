#include <iostream>
#include "allocator.h"
using namespace std;

struct Point {
    int x;
    int y;
};

int main() {
    LinearAllocator allocator(1024);
    
    cout << "=== Free List Allocator Demo ===" << endl;
    cout << "Pool size: 1024 bytes" << endl;
    cout << "BlockHeader size: " << sizeof(BlockHeader) << " bytes" << endl;
    cout << "sizeof(size_t): " << sizeof(size_t) << endl;
    cout << "sizeof(bool): " << sizeof(bool) << endl;
    cout << "sizeof(BlockHeader*): " << sizeof(BlockHeader*) << "\n" << endl;
    
    // Allocate some blocks
    cout << "Allocating 3 integers..." << endl;
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
        cout << "  Offset: " << allocator.get_offset() << " bytes\n" << endl;
    }
    
    // Free middle block
    cout << "Freeing num2..." << endl;
    allocator.free(num2);
    cout << "  Block freed and added to free list\n" << endl;
    
    // Allocate again - should reuse freed block
    cout << "Allocating new integer (should reuse freed block)..." << endl;
    int* num4 = static_cast<int*>(allocator.allocate(sizeof(int)));
    if (num4) {
        *num4 = 40;
        cout << "  num4: " << *num4 << " at " << num4 << endl;
        cout << "  Same address as num2? " << (num4 == num2 ? "YES" : "NO") << endl;
        cout << "  Offset unchanged: " << allocator.get_offset() << " bytes\n" << endl;
    }
    
    // Allocate array
    cout << "Allocating int array[5]..." << endl;
    int* arr = static_cast<int*>(allocator.allocate(sizeof(int) * 5));
    if (arr) {
        for (int i = 0; i < 5; i++) {
            arr[i] = i * 100;
        }
        cout << "  Address: " << arr << endl;
        cout << "  Values: ";
        for (int i = 0; i < 5; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
        cout << "  Offset: " << allocator.get_offset() << " bytes\n" << endl;
    }
    
    // Free and reallocate
    cout << "Freeing array..." << endl;
    allocator.free(arr);
    
    cout << "Allocating Point struct (should reuse array block)..." << endl;
    Point* point = static_cast<Point*>(allocator.allocate(sizeof(Point)));
    if (point) {
        point->x = 100;
        point->y = 200;
        cout << "  Point: (" << point->x << ", " << point->y << ") at " << point << endl;
        cout << "  Reused freed block\n" << endl;
    }
    
    // Test reset
    cout << "Resetting allocator..." << endl;
    allocator.reset();
    cout << "  Offset: " << allocator.get_offset() << endl;
    cout << "  Free list cleared" << endl;
    
    return 0;
}
