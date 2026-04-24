int printf(const char *fmt, ...);

/* Bump Allocator Simulation
 * Uses a flat int array as "memory". A bump pointer tracks the next free slot.
 * Allocations simply advance the pointer. No individual free; only reset.
 */

int heap[256];
int bump_ptr;
int heap_size;

void bump_init(int size) {
    bump_ptr = 0;
    heap_size = size;
    int i;
    for (i = 0; i < 256; i++) {
        heap[i] = 0;
    }
}

int bump_alloc(int count) {
    if (bump_ptr + count > heap_size) {
        return -1;
    }
    int addr = bump_ptr;
    bump_ptr = bump_ptr + count;
    return addr;
}

void bump_reset(void) {
    bump_ptr = 0;
}

int bump_used(void) {
    return bump_ptr;
}

int bump_free_space(void) {
    return heap_size - bump_ptr;
}

void heap_write(int addr, int value) {
    if (addr >= 0 && addr < 256) {
        heap[addr] = value;
    }
}

int heap_read(int addr) {
    if (addr >= 0 && addr < 256) {
        return heap[addr];
    }
    return -1;
}

void test_basic_alloc(void) {
    bump_init(64);
    printf("=== Basic Bump Allocator ===\n"); // EXPECT: === Basic Bump Allocator ===

    int a = bump_alloc(4);
    printf("alloc 4: addr=%d\n", a); // EXPECT: alloc 4: addr=0

    int b = bump_alloc(8);
    printf("alloc 8: addr=%d\n", b); // EXPECT: alloc 8: addr=4

    int c = bump_alloc(2);
    printf("alloc 2: addr=%d\n", c); // EXPECT: alloc 2: addr=12

    printf("used=%d free=%d\n", bump_used(), bump_free_space()); // EXPECT: used=14 free=50

    heap_write(a, 100);
    heap_write(a + 1, 200);
    heap_write(b, 300);
    printf("read a[0]=%d a[1]=%d b[0]=%d\n", heap_read(a), heap_read(a + 1), heap_read(b)); // EXPECT: read a[0]=100 a[1]=200 b[0]=300
}

void test_overflow(void) {
    printf("=== Overflow Test ===\n"); // EXPECT: === Overflow Test ===
    bump_init(10);

    int a = bump_alloc(5);
    printf("alloc 5: addr=%d\n", a); // EXPECT: alloc 5: addr=0

    int b = bump_alloc(5);
    printf("alloc 5: addr=%d\n", b); // EXPECT: alloc 5: addr=5

    int c = bump_alloc(1);
    printf("alloc 1 (should fail): addr=%d\n", c); // EXPECT: alloc 1 (should fail): addr=-1

    printf("used=%d free=%d\n", bump_used(), bump_free_space()); // EXPECT: used=10 free=0
}

void test_reset(void) {
    printf("=== Reset Test ===\n"); // EXPECT: === Reset Test ===
    bump_init(20);

    int a = bump_alloc(10);
    printf("alloc 10: addr=%d used=%d\n", a, bump_used()); // EXPECT: alloc 10: addr=0 used=10

    bump_reset();
    printf("after reset: used=%d free=%d\n", bump_used(), bump_free_space()); // EXPECT: after reset: used=0 free=20

    int b = bump_alloc(15);
    printf("alloc 15: addr=%d used=%d\n", b, bump_used()); // EXPECT: alloc 15: addr=0 used=15

    int c = bump_alloc(10);
    printf("alloc 10 (should fail): addr=%d\n", c); // EXPECT: alloc 10 (should fail): addr=-1
}

void test_many_allocs(void) {
    printf("=== Many Small Allocations ===\n"); // EXPECT: === Many Small Allocations ===
    bump_init(100);

    int count = 0;
    int i;
    for (i = 0; i < 50; i++) {
        int addr = bump_alloc(2);
        if (addr >= 0) {
            heap_write(addr, i * 10);
            count = count + 1;
        }
    }
    printf("successful allocs: %d\n", count); // EXPECT: successful allocs: 50
    printf("used=%d free=%d\n", bump_used(), bump_free_space()); // EXPECT: used=100 free=0

    int sum = 0;
    for (i = 0; i < 50; i++) {
        sum = sum + heap_read(i * 2);
    }
    printf("sum of stored values: %d\n", sum); // EXPECT: sum of stored values: 12250
}

void test_alignment(void) {
    printf("=== Simulated Alignment ===\n"); // EXPECT: === Simulated Alignment ===
    bump_init(64);

    int a = bump_alloc(3);
    printf("alloc 3: addr=%d\n", a); // EXPECT: alloc 3: addr=0

    /* Simulate 4-slot alignment: advance to next multiple of 4 */
    int rem = bump_ptr % 4;
    if (rem != 0) {
        bump_alloc(4 - rem);
    }
    int b = bump_alloc(4);
    printf("aligned alloc 4: addr=%d\n", b); // EXPECT: aligned alloc 4: addr=4
    printf("used=%d\n", bump_used()); // EXPECT: used=8
}

int main(void) {
    test_basic_alloc();
    test_overflow();
    test_reset();
    test_many_allocs();
    test_alignment();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
