#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include "allocator.h"
using namespace std;
using namespace chrono;

// Benchmark 1: Small allocations (where custom allocator shines)
void benchmark_small_allocations() {
    cout << "=== BENCHMARK 1: 100,000 Small Allocations (16 bytes) ===" << endl;
    const int NUM_ALLOCS = 100000;
    const size_t ALLOC_SIZE = 16;
    
    // Custom allocator
    LinearAllocator allocator(NUM_ALLOCS * (ALLOC_SIZE + sizeof(BlockHeader) + 8));
    vector<void*> custom_ptrs;
    
    auto start = high_resolution_clock::now();
    for (int i = 0; i < NUM_ALLOCS; i++) {
        void* ptr = allocator.allocate(ALLOC_SIZE);
        custom_ptrs.push_back(ptr);
    }
    auto end = high_resolution_clock::now();
    auto custom_time = duration_cast<microseconds>(end - start).count();
    
    // malloc
    vector<void*> malloc_ptrs;
    start = high_resolution_clock::now();
    for (int i = 0; i < NUM_ALLOCS; i++) {
        void* ptr = malloc(ALLOC_SIZE);
        malloc_ptrs.push_back(ptr);
    }
    end = high_resolution_clock::now();
    auto malloc_time = duration_cast<microseconds>(end - start).count();
    
    // Cleanup malloc
    for (void* ptr : malloc_ptrs) {
        free(ptr);
    }
    
    cout << "Custom Allocator: " << custom_time << " us" << endl;
    cout << "malloc:           " << malloc_time << " us" << endl;
    
    if (custom_time < malloc_time) {
        double speedup = (double)malloc_time / custom_time;
        cout << "[WIN] Custom allocator is " << fixed << setprecision(2) 
             << speedup << "x FASTER!" << endl;
    } else {
        cout << "malloc is faster (expected for general-purpose allocator)" << endl;
    }
    cout << endl;
}

// Benchmark 2: Allocation + Deallocation with reuse
void benchmark_alloc_dealloc_reuse() {
    cout << "=== BENCHMARK 2: Allocation/Deallocation with Reuse ===" << endl;
    const int NUM_CYCLES = 50000;
    const size_t ALLOC_SIZE = 32;
    
    // Custom allocator
    LinearAllocator allocator(NUM_CYCLES * (ALLOC_SIZE + sizeof(BlockHeader) + 8));
    
    auto start = high_resolution_clock::now();
    for (int i = 0; i < NUM_CYCLES; i++) {
        void* ptr1 = allocator.allocate(ALLOC_SIZE);
        void* ptr2 = allocator.allocate(ALLOC_SIZE);
        allocator.deallocate(ptr1);
        void* ptr3 = allocator.allocate(ALLOC_SIZE); // Should reuse ptr1
        allocator.deallocate(ptr2);
        allocator.deallocate(ptr3);
    }
    auto end = high_resolution_clock::now();
    auto custom_time = duration_cast<microseconds>(end - start).count();
    
    // malloc/free
    start = high_resolution_clock::now();
    for (int i = 0; i < NUM_CYCLES; i++) {
        void* ptr1 = malloc(ALLOC_SIZE);
        void* ptr2 = malloc(ALLOC_SIZE);
        free(ptr1);
        void* ptr3 = malloc(ALLOC_SIZE);
        free(ptr2);
        free(ptr3);
    }
    end = high_resolution_clock::now();
    auto malloc_time = duration_cast<microseconds>(end - start).count();
    
    cout << "Custom Allocator: " << custom_time << " us" << endl;
    cout << "malloc/free:      " << malloc_time << " us" << endl;
    
    if (custom_time < malloc_time) {
        double speedup = (double)malloc_time / custom_time;
        cout << "[WIN] Custom allocator is " << fixed << setprecision(2) 
             << speedup << "x FASTER!" << endl;
    } else {
        cout << "malloc/free is faster" << endl;
    }
    cout << endl;
}

// Benchmark 3: Sequential allocations (custom allocator's strength)
void benchmark_sequential() {
    cout << "=== BENCHMARK 3: Sequential Allocations (Custom Allocator Strength) ===" << endl;
    const int NUM_ALLOCS = 200000;
    const size_t ALLOC_SIZE = 8;
    
    // Custom allocator - sequential bump allocation
    LinearAllocator allocator(NUM_ALLOCS * (ALLOC_SIZE + sizeof(BlockHeader) + 8));
    
    auto start = high_resolution_clock::now();
    for (int i = 0; i < NUM_ALLOCS; i++) {
        allocator.allocate(ALLOC_SIZE);
    }
    auto end = high_resolution_clock::now();
    auto custom_time = duration_cast<microseconds>(end - start).count();
    
    // malloc
    vector<void*> malloc_ptrs;
    start = high_resolution_clock::now();
    for (int i = 0; i < NUM_ALLOCS; i++) {
        void* ptr = malloc(ALLOC_SIZE);
        malloc_ptrs.push_back(ptr);
    }
    end = high_resolution_clock::now();
    auto malloc_time = duration_cast<microseconds>(end - start).count();
    
    // Cleanup
    for (void* ptr : malloc_ptrs) {
        free(ptr);
    }
    
    cout << "Custom Allocator: " << custom_time << " us" << endl;
    cout << "malloc:           " << malloc_time << " us" << endl;
    
    if (custom_time < malloc_time) {
        double speedup = (double)malloc_time / custom_time;
        cout << "[WIN] Custom allocator is " << fixed << setprecision(2) 
             << speedup << "x FASTER!" << endl;
        cout << "Reason: Minimal bookkeeping, no thread safety overhead" << endl;
    } else {
        cout << "malloc is faster" << endl;
    }
    cout << endl;
}

// Benchmark 4: Reset performance (unique to custom allocator)
void benchmark_reset() {
    cout << "=== BENCHMARK 4: Bulk Deallocation (reset vs free) ===" << endl;
    const int NUM_ALLOCS = 100000;
    const size_t ALLOC_SIZE = 16;
    
    // Custom allocator with reset
    LinearAllocator allocator(NUM_ALLOCS * (ALLOC_SIZE + sizeof(BlockHeader) + 8));
    for (int i = 0; i < NUM_ALLOCS; i++) {
        allocator.allocate(ALLOC_SIZE);
    }
    
    auto start = high_resolution_clock::now();
    allocator.reset();
    auto end = high_resolution_clock::now();
    auto reset_time = duration_cast<microseconds>(end - start).count();
    
    // malloc/free
    vector<void*> malloc_ptrs;
    for (int i = 0; i < NUM_ALLOCS; i++) {
        malloc_ptrs.push_back(malloc(ALLOC_SIZE));
    }
    
    start = high_resolution_clock::now();
    for (void* ptr : malloc_ptrs) {
        free(ptr);
    }
    end = high_resolution_clock::now();
    auto free_time = duration_cast<microseconds>(end - start).count();
    
    cout << "Custom reset():   " << reset_time << " us" << endl;
    cout << "malloc free():    " << free_time << " us" << endl;
    
    if (reset_time < free_time) {
        if (reset_time == 0) {
            cout << "[WIN] reset() is INSTANT (too fast to measure)!" << endl;
        } else {
            double speedup = (double)free_time / reset_time;
            cout << "[WIN] reset() is " << fixed << setprecision(2) 
                 << speedup << "x FASTER!" << endl;
        }
        cout << "Reason: O(1) reset vs O(n) individual frees" << endl;
    }
    cout << endl;
}

int main() {
    cout << "========================================================" << endl;
    cout << "     CUSTOM MEMORY ALLOCATOR PERFORMANCE BENCHMARK      " << endl;
    cout << "========================================================" << endl;
    cout << endl;
    
    benchmark_small_allocations();
    benchmark_alloc_dealloc_reuse();
    benchmark_sequential();
    benchmark_reset();
    
    cout << "========================================================" << endl;
    cout << "                    BENCHMARK COMPLETE                  " << endl;
    cout << "========================================================" << endl;
    
    return 0;
}
