int printf(const char *fmt, ...);

// Slab Allocator Simulation
// Fixed-size object caching with multiple slab caches

// Each slab has a fixed object size and a bitmap of allocated slots
// We simulate 3 caches: 16-byte, 32-byte, 64-byte objects

int slab16_bitmap[8];  // 8 slots for 16-byte objects
int slab32_bitmap[8];  // 8 slots for 32-byte objects
int slab64_bitmap[4];  // 4 slots for 64-byte objects

int slab16_alloc_count;
int slab32_alloc_count;
int slab64_alloc_count;

// Object data storage (simulated)
int slab16_data[8];
int slab32_data[8];
int slab64_data[4];

void slab_init() {
    int i;
    for (i = 0; i < 8; i++) {
        slab16_bitmap[i] = 0;
        slab32_bitmap[i] = 0;
        slab16_data[i] = 0;
        slab32_data[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        slab64_bitmap[i] = 0;
        slab64_data[i] = 0;
    }
    slab16_alloc_count = 0;
    slab32_alloc_count = 0;
    slab64_alloc_count = 0;
}

int slab16_alloc(int value) {
    int i;
    for (i = 0; i < 8; i++) {
        if (slab16_bitmap[i] == 0) {
            slab16_bitmap[i] = 1;
            slab16_data[i] = value;
            slab16_alloc_count = slab16_alloc_count + 1;
            return i;
        }
    }
    return -1;
}

int slab32_alloc(int value) {
    int i;
    for (i = 0; i < 8; i++) {
        if (slab32_bitmap[i] == 0) {
            slab32_bitmap[i] = 1;
            slab32_data[i] = value;
            slab32_alloc_count = slab32_alloc_count + 1;
            return i;
        }
    }
    return -1;
}

int slab64_alloc(int value) {
    int i;
    for (i = 0; i < 4; i++) {
        if (slab64_bitmap[i] == 0) {
            slab64_bitmap[i] = 1;
            slab64_data[i] = value;
            slab64_alloc_count = slab64_alloc_count + 1;
            return i;
        }
    }
    return -1;
}

void slab16_free(int slot) {
    if (slot >= 0 && slot < 8 && slab16_bitmap[slot] == 1) {
        slab16_bitmap[slot] = 0;
        slab16_data[slot] = 0;
        slab16_alloc_count = slab16_alloc_count - 1;
    }
}

void slab32_free(int slot) {
    if (slot >= 0 && slot < 8 && slab32_bitmap[slot] == 1) {
        slab32_bitmap[slot] = 0;
        slab32_data[slot] = 0;
        slab32_alloc_count = slab32_alloc_count - 1;
    }
}

void slab64_free(int slot) {
    if (slot >= 0 && slot < 4 && slab64_bitmap[slot] == 1) {
        slab64_bitmap[slot] = 0;
        slab64_data[slot] = 0;
        slab64_alloc_count = slab64_alloc_count - 1;
    }
}

int slab_select_cache(int size) {
    if (size <= 16) return 16;
    if (size <= 32) return 32;
    if (size <= 64) return 64;
    return -1;
}

int count_used_slots(int cache_size) {
    int i;
    int count;
    count = 0;
    if (cache_size == 16) {
        for (i = 0; i < 8; i++) {
            if (slab16_bitmap[i] == 1) count = count + 1;
        }
    } else if (cache_size == 32) {
        for (i = 0; i < 8; i++) {
            if (slab32_bitmap[i] == 1) count = count + 1;
        }
    } else if (cache_size == 64) {
        for (i = 0; i < 4; i++) {
            if (slab64_bitmap[i] == 1) count = count + 1;
        }
    }
    return count;
}

int compute_fragmentation() {
    // Internal fragmentation: allocated space - used space
    // Each 16-byte slot wastes nothing if obj is 16, but say obj is 10 -> waste 6
    // We simplify: fragmentation = total_allocated_capacity - total_alloc_count * min_size
    int total_capacity;
    int total_used;
    total_capacity = slab16_alloc_count * 16 + slab32_alloc_count * 32 + slab64_alloc_count * 64;
    total_used = slab16_alloc_count + slab32_alloc_count + slab64_alloc_count;
    return total_capacity;
}

void print_cache_status() {
    printf("Cache 16B: %d/%d used\n", count_used_slots(16), 8);
    printf("Cache 32B: %d/%d used\n", count_used_slots(32), 8);
    printf("Cache 64B: %d/%d used\n", count_used_slots(64), 4);
}

int main() {
    int s0;
    int s1;
    int s2;
    int s3;
    int s4;

    slab_init();
    printf("Slab Allocator Simulation\n");
    // EXPECT: Slab Allocator Simulation

    // Allocate several 16-byte objects
    s0 = slab16_alloc(100);
    s1 = slab16_alloc(200);
    s2 = slab16_alloc(300);
    printf("Alloc 16B slots: %d %d %d\n", s0, s1, s2);
    // EXPECT: Alloc 16B slots: 0 1 2

    // Allocate 32-byte objects
    s3 = slab32_alloc(400);
    s4 = slab32_alloc(500);
    printf("Alloc 32B slots: %d %d\n", s3, s4);
    // EXPECT: Alloc 32B slots: 0 1

    // Allocate 64-byte object
    int s5;
    s5 = slab64_alloc(600);
    printf("Alloc 64B slot: %d\n", s5);
    // EXPECT: Alloc 64B slot: 0

    print_cache_status();
    // EXPECT: Cache 16B: 3/8 used
    // EXPECT: Cache 32B: 2/8 used
    // EXPECT: Cache 64B: 1/4 used

    // Check data values
    printf("Data at 16B[1]: %d\n", slab16_data[1]);
    // EXPECT: Data at 16B[1]: 200
    printf("Data at 32B[0]: %d\n", slab32_data[0]);
    // EXPECT: Data at 32B[0]: 400

    // Free and reallocate
    slab16_free(1);
    printf("Freed 16B slot 1\n");
    // EXPECT: Freed 16B slot 1

    int s6;
    s6 = slab16_alloc(999);
    printf("Realloc 16B slot: %d, value: %d\n", s6, slab16_data[s6]);
    // EXPECT: Realloc 16B slot: 1, value: 999

    // Cache selection
    printf("Size 10 -> cache %d\n", slab_select_cache(10));
    // EXPECT: Size 10 -> cache 16
    printf("Size 20 -> cache %d\n", slab_select_cache(20));
    // EXPECT: Size 20 -> cache 32
    printf("Size 50 -> cache %d\n", slab_select_cache(50));
    // EXPECT: Size 50 -> cache 64
    printf("Size 100 -> cache %d\n", slab_select_cache(100));
    // EXPECT: Size 100 -> cache -1

    // Fill up the 64-byte cache
    int s7;
    int s8;
    int s9;
    int s10;
    s7 = slab64_alloc(700);
    s8 = slab64_alloc(800);
    s9 = slab64_alloc(900);
    s10 = slab64_alloc(1000); // should fail
    printf("64B fill: %d %d %d %d\n", s7, s8, s9, s10);
    // EXPECT: 64B fill: 1 2 3 -1

    print_cache_status();
    // EXPECT: Cache 16B: 3/8 used
    // EXPECT: Cache 32B: 2/8 used
    // EXPECT: Cache 64B: 4/4 used

    // Total capacity allocated
    int cap;
    cap = compute_fragmentation();
    printf("Total allocated capacity: %d bytes\n", cap);
    // EXPECT: Total allocated capacity: 368 bytes

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
