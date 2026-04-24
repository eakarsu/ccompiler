int printf(const char *fmt, ...);

// Best-fit memory allocator simulation
// Finds the smallest free block that fits the request

struct Block {
    int start;
    int size;
    int used;
    int pid;
};

struct BestFitAlloc {
    struct Block blocks[20];
    int count;
};

void bfa_init(struct BestFitAlloc *a, int size) {
    a->count = 1;
    a->blocks[0].start = 0;
    a->blocks[0].size = size;
    a->blocks[0].used = 0;
    a->blocks[0].pid = -1;
}

int bfa_alloc(struct BestFitAlloc *a, int size, int pid) {
    int best = -1;
    int best_size = 999999;
    int i;

    // Find smallest free block that fits
    for (i = 0; i < a->count; i++) {
        if (!a->blocks[i].used && a->blocks[i].size >= size) {
            if (a->blocks[i].size < best_size) {
                best_size = a->blocks[i].size;
                best = i;
            }
        }
    }

    if (best == -1) return -1;

    // Split if needed
    if (a->blocks[best].size > size) {
        int j;
        for (j = a->count; j > best + 1; j--) {
            a->blocks[j] = a->blocks[j - 1];
        }
        a->blocks[best + 1].start = a->blocks[best].start + size;
        a->blocks[best + 1].size = a->blocks[best].size - size;
        a->blocks[best + 1].used = 0;
        a->blocks[best + 1].pid = -1;
        a->count++;
    }

    a->blocks[best].size = size;
    a->blocks[best].used = 1;
    a->blocks[best].pid = pid;
    return a->blocks[best].start;
}

void bfa_free(struct BestFitAlloc *a, int pid) {
    int i;
    for (i = 0; i < a->count; i++) {
        if (a->blocks[i].used && a->blocks[i].pid == pid) {
            a->blocks[i].used = 0;
            a->blocks[i].pid = -1;
            break;
        }
    }
    // Coalesce
    i = 0;
    while (i < a->count - 1) {
        if (!a->blocks[i].used && !a->blocks[i + 1].used) {
            a->blocks[i].size = a->blocks[i].size + a->blocks[i + 1].size;
            int j;
            for (j = i + 1; j < a->count - 1; j++) {
                a->blocks[j] = a->blocks[j + 1];
            }
            a->count--;
        } else {
            i++;
        }
    }
}

int bfa_total_free(struct BestFitAlloc *a) {
    int t = 0;
    int i;
    for (i = 0; i < a->count; i++) {
        if (!a->blocks[i].used) t = t + a->blocks[i].size;
    }
    return t;
}

int bfa_largest_free(struct BestFitAlloc *a) {
    int mx = 0;
    int i;
    for (i = 0; i < a->count; i++) {
        if (!a->blocks[i].used && a->blocks[i].size > mx) {
            mx = a->blocks[i].size;
        }
    }
    return mx;
}

int bfa_count_free(struct BestFitAlloc *a) {
    int c = 0;
    int i;
    for (i = 0; i < a->count; i++) {
        if (!a->blocks[i].used) c++;
    }
    return c;
}

int main(void) {
    struct BestFitAlloc alloc;
    bfa_init(&alloc, 500);

    printf("Init: total=%d\n", bfa_total_free(&alloc));
    // EXPECT: Init: total=500

    // Allocate several blocks
    int a1 = bfa_alloc(&alloc, 100, 1);
    int a2 = bfa_alloc(&alloc, 50, 2);
    int a3 = bfa_alloc(&alloc, 200, 3);
    int a4 = bfa_alloc(&alloc, 30, 4);

    printf("a1=%d a2=%d a3=%d a4=%d\n", a1, a2, a3, a4);
    // EXPECT: a1=0 a2=100 a3=150 a4=350

    printf("Free: %d, Blocks: %d\n", bfa_total_free(&alloc), alloc.count);
    // EXPECT: Free: 120, Blocks: 5

    // Free pid 2 (size 50) and pid 4 (size 30)
    bfa_free(&alloc, 2);
    bfa_free(&alloc, 4);
    printf("After freeing 2,4: free=%d free_blocks=%d\n",
           bfa_total_free(&alloc), bfa_count_free(&alloc));
    // EXPECT: After freeing 2,4: free=200 free_blocks=2

    // After coalescing: [100-149]=50, [350-499]=150
    // Alloc 25 bytes - best fit chooses the 50-byte hole (smallest >= 25)
    int a5 = bfa_alloc(&alloc, 25, 5);
    printf("Best fit 25 -> addr=%d\n", a5);
    // EXPECT: Best fit 25 -> addr=100

    // Now: [125-149]=25 free, [350-499]=150 free
    // Alloc 45 -> only 150 fits (25 < 45)
    int a6 = bfa_alloc(&alloc, 45, 6);
    printf("Best fit 45 -> addr=%d\n", a6);
    // EXPECT: Best fit 45 -> addr=350

    // Now: [125-149]=25 free, [395-499]=105 free
    printf("Free blocks: %d, Total free: %d\n",
           bfa_count_free(&alloc), bfa_total_free(&alloc));
    // EXPECT: Free blocks: 2, Total free: 130

    // Alloc 5 -> best fit picks 25-byte hole (smallest >= 5)
    int a7 = bfa_alloc(&alloc, 5, 7);
    printf("Best fit 5 -> addr=%d\n", a7);
    // EXPECT: Best fit 5 -> addr=125

    // Now: [130-149]=20 free, [395-499]=105 free
    // Alloc 5 -> best fit picks 20-byte hole
    int a8 = bfa_alloc(&alloc, 5, 8);
    printf("Best fit 5 -> addr=%d\n", a8);
    // EXPECT: Best fit 5 -> addr=130

    printf("Free: %d, Largest: %d\n", bfa_total_free(&alloc), bfa_largest_free(&alloc));
    // EXPECT: Free: 120, Largest: 105

    // Can't fit 121 (largest free is 105)
    int fail = bfa_alloc(&alloc, 121, 9);
    printf("Alloc 121: %d\n", fail);
    // EXPECT: Alloc 121: -1

    // Free everything
    bfa_free(&alloc, 1);
    bfa_free(&alloc, 3);
    bfa_free(&alloc, 5);
    bfa_free(&alloc, 6);
    bfa_free(&alloc, 7);
    bfa_free(&alloc, 8);
    printf("All freed: total=%d blocks=%d\n", bfa_total_free(&alloc), alloc.count);
    // EXPECT: All freed: total=500 blocks=1

    // Test exact fit preference
    bfa_alloc(&alloc, 100, 20);
    bfa_alloc(&alloc, 100, 21);
    bfa_alloc(&alloc, 100, 22);
    bfa_free(&alloc, 21); // creates 100-byte hole in middle
    // holes: [100-199]=100, [300-499]=200
    int exact = bfa_alloc(&alloc, 100, 23);
    printf("Exact fit 100 -> addr=%d\n", exact);
    // EXPECT: Exact fit 100 -> addr=100

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
