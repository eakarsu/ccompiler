int printf(const char *fmt, ...);

// First-fit memory allocator simulation
// Manages a fixed pool of memory blocks

struct Block {
    int start;
    int size;
    int used;
    int pid;
};

struct Allocator {
    struct Block blocks[16];
    int count;
    int total_size;
};

void init_allocator(struct Allocator *a, int size) {
    a->count = 1;
    a->total_size = size;
    a->blocks[0].start = 0;
    a->blocks[0].size = size;
    a->blocks[0].used = 0;
    a->blocks[0].pid = -1;
}

int first_fit_alloc(struct Allocator *a, int size, int pid) {
    int i;
    for (i = 0; i < a->count; i++) {
        if (!a->blocks[i].used && a->blocks[i].size >= size) {
            // Found a free block big enough
            if (a->blocks[i].size > size) {
                // Split: shift blocks right
                int j;
                for (j = a->count; j > i + 1; j--) {
                    a->blocks[j] = a->blocks[j - 1];
                }
                a->blocks[i + 1].start = a->blocks[i].start + size;
                a->blocks[i + 1].size = a->blocks[i].size - size;
                a->blocks[i + 1].used = 0;
                a->blocks[i + 1].pid = -1;
                a->count++;
            }
            a->blocks[i].size = size;
            a->blocks[i].used = 1;
            a->blocks[i].pid = pid;
            return a->blocks[i].start;
        }
    }
    return -1; // no space
}

void first_fit_free(struct Allocator *a, int pid) {
    int i;
    for (i = 0; i < a->count; i++) {
        if (a->blocks[i].used && a->blocks[i].pid == pid) {
            a->blocks[i].used = 0;
            a->blocks[i].pid = -1;
            break;
        }
    }
    // Coalesce adjacent free blocks
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

int count_free_blocks(struct Allocator *a) {
    int count = 0;
    int i;
    for (i = 0; i < a->count; i++) {
        if (!a->blocks[i].used) count++;
    }
    return count;
}

int total_free(struct Allocator *a) {
    int total = 0;
    int i;
    for (i = 0; i < a->count; i++) {
        if (!a->blocks[i].used) total = total + a->blocks[i].size;
    }
    return total;
}

int largest_free(struct Allocator *a) {
    int largest = 0;
    int i;
    for (i = 0; i < a->count; i++) {
        if (!a->blocks[i].used && a->blocks[i].size > largest) {
            largest = a->blocks[i].size;
        }
    }
    return largest;
}

void print_blocks(struct Allocator *a) {
    int i;
    for (i = 0; i < a->count; i++) {
        if (a->blocks[i].used) {
            printf("  [%d-%d] pid=%d\n", a->blocks[i].start,
                   a->blocks[i].start + a->blocks[i].size - 1,
                   a->blocks[i].pid);
        } else {
            printf("  [%d-%d] free\n", a->blocks[i].start,
                   a->blocks[i].start + a->blocks[i].size - 1);
        }
    }
}

int main(void) {
    struct Allocator alloc;
    init_allocator(&alloc, 1000);

    printf("Initial: %d blocks, %d free\n", alloc.count, total_free(&alloc));
    // EXPECT: Initial: 1 blocks, 1000 free

    // Allocate for pid 1: 100 bytes
    int addr1 = first_fit_alloc(&alloc, 100, 1);
    printf("Alloc pid=1 size=100 at %d\n", addr1);
    // EXPECT: Alloc pid=1 size=100 at 0

    // Allocate for pid 2: 200 bytes
    int addr2 = first_fit_alloc(&alloc, 200, 2);
    printf("Alloc pid=2 size=200 at %d\n", addr2);
    // EXPECT: Alloc pid=2 size=200 at 100

    // Allocate for pid 3: 150 bytes
    int addr3 = first_fit_alloc(&alloc, 150, 3);
    printf("Alloc pid=3 size=150 at %d\n", addr3);
    // EXPECT: Alloc pid=3 size=150 at 300

    printf("Blocks: %d, Free: %d\n", alloc.count, total_free(&alloc));
    // EXPECT: Blocks: 4, Free: 550

    // Free pid 2 (middle block)
    first_fit_free(&alloc, 2);
    printf("After freeing pid=2: blocks=%d free=%d\n", alloc.count, total_free(&alloc));
    // EXPECT: After freeing pid=2: blocks=4 free=750

    // Allocate pid 4: 80 bytes - should go into freed slot (100-299)
    int addr4 = first_fit_alloc(&alloc, 80, 4);
    printf("Alloc pid=4 size=80 at %d\n", addr4);
    // EXPECT: Alloc pid=4 size=80 at 100

    printf("Blocks: %d, Free: %d, Largest: %d\n",
           alloc.count, total_free(&alloc), largest_free(&alloc));
    // EXPECT: Blocks: 5, Free: 670, Largest: 550

    // Free pid 1 and pid 4 - should coalesce
    first_fit_free(&alloc, 1);
    first_fit_free(&alloc, 4);
    printf("After freeing 1,4: blocks=%d free=%d\n", alloc.count, total_free(&alloc));
    // EXPECT: After freeing 1,4: blocks=3 free=850

    printf("Largest free: %d\n", largest_free(&alloc));
    // EXPECT: Largest free: 550

    // Free pid 3 - should coalesce with block after it and block before
    first_fit_free(&alloc, 3);
    printf("After freeing 3: blocks=%d free=%d\n", alloc.count, total_free(&alloc));
    // EXPECT: After freeing 3: blocks=1 free=1000

    // Stress: fill memory
    first_fit_alloc(&alloc, 250, 10);
    first_fit_alloc(&alloc, 250, 11);
    first_fit_alloc(&alloc, 250, 12);
    first_fit_alloc(&alloc, 250, 13);
    int fail = first_fit_alloc(&alloc, 1, 14);
    printf("Full alloc attempt: %d\n", fail);
    // EXPECT: Full alloc attempt: -1

    printf("Free blocks: %d\n", count_free_blocks(&alloc));
    // EXPECT: Free blocks: 0

    // Free alternating
    first_fit_free(&alloc, 11);
    first_fit_free(&alloc, 13);
    printf("After alternating free: free_blocks=%d total_free=%d\n",
           count_free_blocks(&alloc), total_free(&alloc));
    // EXPECT: After alternating free: free_blocks=2 total_free=500

    // Can't fit 300 even though 500 total free (fragmentation!)
    int frag = first_fit_alloc(&alloc, 300, 15);
    printf("Fragmented alloc 300: %d\n", frag);
    // EXPECT: Fragmented alloc 300: -1

    // But 250 fits
    int fit = first_fit_alloc(&alloc, 250, 16);
    printf("Fit alloc 250: %d\n", fit);
    // EXPECT: Fit alloc 250: 250

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
