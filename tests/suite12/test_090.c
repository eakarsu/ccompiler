int printf(const char *fmt, ...);

// Memory Fragmentation Analyzer
// Simulates heap allocation/deallocation and measures fragmentation metrics

// Heap represented as array of blocks
// Each block: status (0=free, 1=used), size
int heap_status[64];    // 0=free, 1=used
int heap_size[64];      // size of each block
int heap_count;         // number of blocks

int total_capacity;
int alloc_counter;

void frag_init() {
    int i;
    for (i = 0; i < 64; i++) {
        heap_status[i] = 0;
        heap_size[i] = 0;
    }
    // Start with one big free block
    heap_status[0] = 0;
    heap_size[0] = 1024;
    heap_count = 1;
    total_capacity = 1024;
    alloc_counter = 0;
}

// First-fit allocation: find first free block >= size, split if needed
int frag_alloc(int size) {
    int i;
    for (i = 0; i < heap_count; i++) {
        if (heap_status[i] == 0 && heap_size[i] >= size) {
            int remaining;
            remaining = heap_size[i] - size;

            if (remaining > 0 && heap_count < 63) {
                // Split: shift blocks right to make room for remainder
                int j;
                for (j = heap_count; j > i + 1; j--) {
                    heap_status[j] = heap_status[j - 1];
                    heap_size[j] = heap_size[j - 1];
                }
                heap_status[i] = 1;
                heap_size[i] = size;
                heap_status[i + 1] = 0;
                heap_size[i + 1] = remaining;
                heap_count = heap_count + 1;
            } else {
                heap_status[i] = 1;
            }
            alloc_counter = alloc_counter + 1;
            return i;
        }
    }
    return -1; // no fit
}

// Free a block and coalesce with neighbors
int frag_free(int idx) {
    if (idx < 0 || idx >= heap_count) return -1;
    if (heap_status[idx] == 0) return -1;

    heap_status[idx] = 0;

    // Coalesce with next block if free
    if (idx + 1 < heap_count && heap_status[idx + 1] == 0) {
        heap_size[idx] = heap_size[idx] + heap_size[idx + 1];
        // Remove block idx+1 by shifting left
        int j;
        for (j = idx + 1; j < heap_count - 1; j++) {
            heap_status[j] = heap_status[j + 1];
            heap_size[j] = heap_size[j + 1];
        }
        heap_count = heap_count - 1;
    }

    // Coalesce with previous block if free
    if (idx > 0 && heap_status[idx - 1] == 0) {
        heap_size[idx - 1] = heap_size[idx - 1] + heap_size[idx];
        int j;
        for (j = idx; j < heap_count - 1; j++) {
            heap_status[j] = heap_status[j + 1];
            heap_size[j] = heap_size[j + 1];
        }
        heap_count = heap_count - 1;
    }

    return 0;
}

int count_free_blocks() {
    int i;
    int count;
    count = 0;
    for (i = 0; i < heap_count; i++) {
        if (heap_status[i] == 0) count = count + 1;
    }
    return count;
}

int count_used_blocks() {
    int i;
    int count;
    count = 0;
    for (i = 0; i < heap_count; i++) {
        if (heap_status[i] == 1) count = count + 1;
    }
    return count;
}

int total_free_space() {
    int i;
    int total;
    total = 0;
    for (i = 0; i < heap_count; i++) {
        if (heap_status[i] == 0) total = total + heap_size[i];
    }
    return total;
}

int total_used_space() {
    int i;
    int total;
    total = 0;
    for (i = 0; i < heap_count; i++) {
        if (heap_status[i] == 1) total = total + heap_size[i];
    }
    return total;
}

int largest_free_block() {
    int i;
    int max_size;
    max_size = 0;
    for (i = 0; i < heap_count; i++) {
        if (heap_status[i] == 0 && heap_size[i] > max_size) {
            max_size = heap_size[i];
        }
    }
    return max_size;
}

// External fragmentation: 1 - (largest_free / total_free) expressed as percentage
int external_frag_pct() {
    int tf;
    int lf;
    tf = total_free_space();
    lf = largest_free_block();
    if (tf == 0) return 0;
    return 100 - (lf * 100) / tf;
}

void print_heap() {
    int i;
    printf("Heap (%d blocks):", heap_count);
    for (i = 0; i < heap_count; i++) {
        if (heap_status[i] == 1) {
            printf(" [U%d]", heap_size[i]);
        } else {
            printf(" [F%d]", heap_size[i]);
        }
    }
    printf("\n");
}

int main() {
    int a;
    int b;
    int c;
    int d;
    int e;

    frag_init();
    printf("Memory Fragmentation Analyzer\n");
    // EXPECT: Memory Fragmentation Analyzer

    printf("Initial: free=%d, used=%d\n", total_free_space(), total_used_space());
    // EXPECT: Initial: free=1024, used=0

    // Allocate several blocks
    a = frag_alloc(100);
    b = frag_alloc(200);
    c = frag_alloc(50);
    d = frag_alloc(150);
    e = frag_alloc(100);
    printf("Allocated 5 blocks\n");
    // EXPECT: Allocated 5 blocks

    print_heap();
    // EXPECT: Heap (6 blocks): [U100] [U200] [U50] [U150] [U100] [F424]

    printf("Free: %d, Used: %d\n", total_free_space(), total_used_space());
    // EXPECT: Free: 424, Used: 600
    printf("Frag: %d\n", external_frag_pct());
    // EXPECT: Frag: 0

    // Free alternating blocks to create fragmentation
    frag_free(b);  // free block at index 1 (200 bytes)
    frag_free(d);  // free block at index 3 (but shifted after coalesce check)
    printf("Freed b and d\n");
    // EXPECT: Freed b and d

    print_heap();
    // EXPECT: Heap (6 blocks): [U100] [F200] [U50] [F150] [U100] [F424]

    printf("Free blocks: %d\n", count_free_blocks());
    // EXPECT: Free blocks: 3
    printf("Free space: %d, largest: %d\n", total_free_space(), largest_free_block());
    // EXPECT: Free space: 774, largest: 424
    printf("Ext frag: %d\n", external_frag_pct());
    // EXPECT: Ext frag: 46

    // Try to allocate 500 bytes - fails despite 774 free (fragmented!)
    int f;
    f = frag_alloc(500);
    printf("Alloc 500 (fragmented): %d\n", f);
    // EXPECT: Alloc 500 (fragmented): -1

    // Allocate something that fits in a hole
    int g;
    g = frag_alloc(100);
    printf("Alloc 100 in hole: block %d\n", g);
    // EXPECT: Alloc 100 in hole: block 1

    print_heap();
    // EXPECT: Heap (7 blocks): [U100] [U100] [F100] [U50] [F150] [U100] [F424]

    // Free adjacent blocks to test coalescing
    frag_free(a);  // free block 0
    printf("Freed block 0\n");
    // EXPECT: Freed block 0

    print_heap();
    // EXPECT: Heap (7 blocks): [F100] [U100] [F100] [U50] [F150] [U100] [F424]

    frag_free(g);  // free block 1 (between two free blocks)
    printf("Freed block 1 (coalesce)\n");
    // EXPECT: Freed block 1 (coalesce)

    print_heap();
    // EXPECT: Heap (5 blocks): [F300] [U50] [F150] [U100] [F424]

    printf("After coalesce: free blocks=%d, largest=%d\n",
           count_free_blocks(), largest_free_block());
    // EXPECT: After coalesce: free blocks=3, largest=424
    printf("Ext frag: %d\n", external_frag_pct());
    // EXPECT: Ext frag: 52

    // Now 500 byte alloc should fit in the 424... no, try 280
    int h;
    h = frag_alloc(280);
    printf("Alloc 280: block %d\n", h);
    // EXPECT: Alloc 280: block 0

    print_heap();
    // EXPECT: Heap (6 blocks): [U280] [F20] [U50] [F150] [U100] [F424]

    printf("Total blocks: %d, free: %d, used: %d\n",
           heap_count, count_free_blocks(), count_used_blocks());
    // EXPECT: Total blocks: 6, free: 3, used: 3

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
