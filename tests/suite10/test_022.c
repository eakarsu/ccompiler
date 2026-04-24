int printf(const char *fmt, ...);

// Memory Allocator Simulation: First-Fit and Best-Fit

struct MemBlock {
    int start;
    int size;
    int free;
    int pid;
};

struct MemAllocator {
    struct MemBlock blocks[20];
    int num_blocks;
    int total_size;
};

void init_allocator(struct MemAllocator *a, int total) {
    a->num_blocks = 1;
    a->total_size = total;
    a->blocks[0].start = 0;
    a->blocks[0].size = total;
    a->blocks[0].free = 1;
    a->blocks[0].pid = -1;
}

int first_fit_alloc(struct MemAllocator *a, int pid, int size) {
    int i;
    for (i = 0; i < a->num_blocks; i = i + 1) {
        if (a->blocks[i].free && a->blocks[i].size >= size) {
            int remaining;
            remaining = a->blocks[i].size - size;
            a->blocks[i].free = 0;
            a->blocks[i].pid = pid;
            if (remaining > 0) {
                // Split block: shift everything right
                int j;
                for (j = a->num_blocks; j > i + 1; j = j - 1) {
                    a->blocks[j] = a->blocks[j - 1];
                }
                a->blocks[i].size = size;
                a->blocks[i + 1].start = a->blocks[i].start + size;
                a->blocks[i + 1].size = remaining;
                a->blocks[i + 1].free = 1;
                a->blocks[i + 1].pid = -1;
                a->num_blocks = a->num_blocks + 1;
            }
            return a->blocks[i].start;
        }
    }
    return -1;
}

int best_fit_alloc(struct MemAllocator *a, int pid, int size) {
    int i;
    int best_idx;
    int best_size;
    best_idx = -1;
    best_size = 99999;
    for (i = 0; i < a->num_blocks; i = i + 1) {
        if (a->blocks[i].free && a->blocks[i].size >= size) {
            if (a->blocks[i].size < best_size) {
                best_size = a->blocks[i].size;
                best_idx = i;
            }
        }
    }
    if (best_idx == -1) return -1;
    {
        int remaining;
        remaining = a->blocks[best_idx].size - size;
        a->blocks[best_idx].free = 0;
        a->blocks[best_idx].pid = pid;
        if (remaining > 0) {
            int j;
            for (j = a->num_blocks; j > best_idx + 1; j = j - 1) {
                a->blocks[j] = a->blocks[j - 1];
            }
            a->blocks[best_idx].size = size;
            a->blocks[best_idx + 1].start = a->blocks[best_idx].start + size;
            a->blocks[best_idx + 1].size = remaining;
            a->blocks[best_idx + 1].free = 1;
            a->blocks[best_idx + 1].pid = -1;
            a->num_blocks = a->num_blocks + 1;
        }
    }
    return a->blocks[best_idx].start;
}

void mem_free(struct MemAllocator *a, int pid) {
    int i;
    for (i = 0; i < a->num_blocks; i = i + 1) {
        if (a->blocks[i].pid == pid) {
            a->blocks[i].free = 1;
            a->blocks[i].pid = -1;
            // Coalesce with next
            if (i + 1 < a->num_blocks && a->blocks[i + 1].free) {
                a->blocks[i].size = a->blocks[i].size + a->blocks[i + 1].size;
                int j;
                for (j = i + 1; j < a->num_blocks - 1; j = j + 1) {
                    a->blocks[j] = a->blocks[j + 1];
                }
                a->num_blocks = a->num_blocks - 1;
            }
            // Coalesce with prev
            if (i > 0 && a->blocks[i - 1].free) {
                a->blocks[i - 1].size = a->blocks[i - 1].size + a->blocks[i].size;
                int j;
                for (j = i; j < a->num_blocks - 1; j = j + 1) {
                    a->blocks[j] = a->blocks[j + 1];
                }
                a->num_blocks = a->num_blocks - 1;
            }
            return;
        }
    }
}

void print_mem(struct MemAllocator *a) {
    int i;
    for (i = 0; i < a->num_blocks; i = i + 1) {
        if (i > 0) printf(" ");
        if (a->blocks[i].free) {
            printf("[FREE start=%d size=%d]", a->blocks[i].start, a->blocks[i].size);
        } else {
            printf("[P%d start=%d size=%d]", a->blocks[i].pid, a->blocks[i].start, a->blocks[i].size);
        }
    }
    printf("\n");
}

int main() {
    struct MemAllocator alloc;
    int addr;

    printf("=== First-Fit Allocator ===\n"); // EXPECT: === First-Fit Allocator ===
    init_allocator(&alloc, 100);

    addr = first_fit_alloc(&alloc, 1, 20);
    printf("Alloc P1(20) at %d\n", addr); // EXPECT: Alloc P1(20) at 0
    addr = first_fit_alloc(&alloc, 2, 30);
    printf("Alloc P2(30) at %d\n", addr); // EXPECT: Alloc P2(30) at 20
    addr = first_fit_alloc(&alloc, 3, 15);
    printf("Alloc P3(15) at %d\n", addr); // EXPECT: Alloc P3(15) at 50

    print_mem(&alloc); // EXPECT: [P1 start=0 size=20] [P2 start=20 size=30] [P3 start=50 size=15] [FREE start=65 size=35]

    mem_free(&alloc, 2);
    printf("Freed P2\n"); // EXPECT: Freed P2
    print_mem(&alloc); // EXPECT: [P1 start=0 size=20] [FREE start=20 size=30] [P3 start=50 size=15] [FREE start=65 size=35]

    addr = first_fit_alloc(&alloc, 4, 10);
    printf("Alloc P4(10) at %d\n", addr); // EXPECT: Alloc P4(10) at 20

    printf("=== Best-Fit Allocator ===\n"); // EXPECT: === Best-Fit Allocator ===
    init_allocator(&alloc, 100);

    addr = best_fit_alloc(&alloc, 1, 20);
    printf("Alloc P1(20) at %d\n", addr); // EXPECT: Alloc P1(20) at 0
    addr = best_fit_alloc(&alloc, 2, 30);
    printf("Alloc P2(30) at %d\n", addr); // EXPECT: Alloc P2(30) at 20
    addr = best_fit_alloc(&alloc, 3, 10);
    printf("Alloc P3(10) at %d\n", addr); // EXPECT: Alloc P3(10) at 50
    mem_free(&alloc, 2);
    // Now: [P1 20] [FREE 30] [P3 10] [FREE 40]
    addr = best_fit_alloc(&alloc, 4, 25);
    printf("Best-fit P4(25) at %d\n", addr); // EXPECT: Best-fit P4(25) at 20
    print_mem(&alloc); // EXPECT: [P1 start=0 size=20] [P4 start=20 size=25] [FREE start=45 size=5] [P3 start=50 size=10] [FREE start=60 size=40]

    printf("Memory allocator simulation complete\n"); // EXPECT: Memory allocator simulation complete
    return 0;
}
