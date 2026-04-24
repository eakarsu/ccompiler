int printf(const char *fmt, ...);
// EXPECT: === Memory Hierarchy Simulation ===\nTotal memory: 1024\nAfter allocations:\n  [  0-127] size=128 USED (owner=1)\n  [128-383] size=256 USED (owner=2)\n  [384-447] size= 64 USED (owner=3)\n  [448-639] size=192 USED (owner=4)\n  [640-767] size=128 USED (owner=5)\n  [768-1023] size=256 FREE\nAllocated: 768 / 1024\nFragments: 1\nFreeing blocks at 128 and 448\nAfter frees:\n  [  0-127] size=128 USED (owner=1)\n  [128-383] size=256 FREE\n  [384-447] size= 64 USED (owner=3)\n  [448-639] size=192 FREE\n  [640-767] size=128 USED (owner=5)\n  [768-1023] size=256 FREE\nFragments: 3\nAlloc 100 for owner 6 at 128\nBefore coalesce:\n  [  0-127] size=128 USED (owner=1)\n  [128-227] size=100 USED (owner=6)\n  [228-383] size=156 FREE\n  [384-447] size= 64 USED (owner=3)\n  [448-639] size=192 FREE\n  [640-767] size=128 USED (owner=5)\n  [768-1023] size=256 FREE\nAfter free(384) + coalesce:\n  [  0-127] size=128 USED (owner=1)\n  [128-227] size=100 USED (owner=6)\n  [228-639] size=412 FREE\n  [640-767] size=128 USED (owner=5)\n  [768-1023] size=256 FREE\nFragments: 2\nAllocated: 356 / 1024

struct MemoryBlock {
    int start_addr;
    int size;
    int allocated;
    int owner_id;
};

struct MemoryManager {
    struct MemoryBlock blocks[16];
    int num_blocks;
    int total_size;
    int allocated_size;
};

void init_memory(struct MemoryManager *mm, int total) {
    mm->num_blocks = 1;
    mm->total_size = total;
    mm->allocated_size = 0;
    mm->blocks[0].start_addr = 0;
    mm->blocks[0].size = total;
    mm->blocks[0].allocated = 0;
    mm->blocks[0].owner_id = -1;
    int i;
    for (i = 1; i < 16; i++) {
        mm->blocks[i].size = 0;
        mm->blocks[i].allocated = 0;
    }
}

int find_first_fit(struct MemoryManager *mm, int size) {
    int i;
    for (i = 0; i < mm->num_blocks; i++) {
        if (!mm->blocks[i].allocated && mm->blocks[i].size >= size) {
            return i;
        }
    }
    return -1;
}

int mem_alloc(struct MemoryManager *mm, int size, int owner) {
    int idx = find_first_fit(mm, size);
    if (idx < 0) return -1;

    if (mm->blocks[idx].size > size && mm->num_blocks < 15) {
        int i;
        for (i = mm->num_blocks; i > idx + 1; i--) {
            mm->blocks[i] = mm->blocks[i - 1];
        }
        mm->blocks[idx + 1].start_addr = mm->blocks[idx].start_addr + size;
        mm->blocks[idx + 1].size = mm->blocks[idx].size - size;
        mm->blocks[idx + 1].allocated = 0;
        mm->blocks[idx + 1].owner_id = -1;
        mm->blocks[idx].size = size;
        mm->num_blocks++;
    }

    mm->blocks[idx].allocated = 1;
    mm->blocks[idx].owner_id = owner;
    mm->allocated_size = mm->allocated_size + size;
    return mm->blocks[idx].start_addr;
}

int mem_free(struct MemoryManager *mm, int addr) {
    int i;
    for (i = 0; i < mm->num_blocks; i++) {
        if (mm->blocks[i].start_addr == addr && mm->blocks[i].allocated) {
            mm->allocated_size = mm->allocated_size - mm->blocks[i].size;
            mm->blocks[i].allocated = 0;
            mm->blocks[i].owner_id = -1;
            return mm->blocks[i].size;
        }
    }
    return -1;
}

void coalesce(struct MemoryManager *mm) {
    int i = 0;
    while (i < mm->num_blocks - 1) {
        if (!mm->blocks[i].allocated && !mm->blocks[i + 1].allocated) {
            mm->blocks[i].size = mm->blocks[i].size + mm->blocks[i + 1].size;
            int j;
            for (j = i + 1; j < mm->num_blocks - 1; j++) {
                mm->blocks[j] = mm->blocks[j + 1];
            }
            mm->num_blocks--;
        } else {
            i++;
        }
    }
}

int count_fragments(struct MemoryManager *mm) {
    int frags = 0;
    int i;
    for (i = 0; i < mm->num_blocks; i++) {
        if (!mm->blocks[i].allocated) frags++;
    }
    return frags;
}

void print_memory(struct MemoryManager *mm) {
    int i;
    for (i = 0; i < mm->num_blocks; i++) {
        printf("  [%3d-%3d] size=%3d %s",
               mm->blocks[i].start_addr,
               mm->blocks[i].start_addr + mm->blocks[i].size - 1,
               mm->blocks[i].size,
               mm->blocks[i].allocated ? "USED" : "FREE");
        if (mm->blocks[i].allocated)
            printf(" (owner=%d)", mm->blocks[i].owner_id);
        printf("\n");
    }
}

int main(void) {
    struct MemoryManager mm;
    init_memory(&mm, 1024);

    printf("=== Memory Hierarchy Simulation ===\n");
    printf("Total memory: %d\n", mm.total_size);

    int a1 = mem_alloc(&mm, 128, 1);
    int a2 = mem_alloc(&mm, 256, 2);
    int a3 = mem_alloc(&mm, 64, 3);
    int a4 = mem_alloc(&mm, 192, 4);
    int a5 = mem_alloc(&mm, 128, 5);

    printf("After allocations:\n");
    print_memory(&mm);
    printf("Allocated: %d / %d\n", mm.allocated_size, mm.total_size);
    printf("Fragments: %d\n", count_fragments(&mm));

    printf("Freeing blocks at %d and %d\n", a2, a4);
    mem_free(&mm, a2);
    mem_free(&mm, a4);

    printf("After frees:\n");
    print_memory(&mm);
    printf("Fragments: %d\n", count_fragments(&mm));

    int a6 = mem_alloc(&mm, 100, 6);
    printf("Alloc 100 for owner 6 at %d\n", a6);

    printf("Before coalesce:\n");
    print_memory(&mm);

    mem_free(&mm, a3);
    coalesce(&mm);
    printf("After free(%d) + coalesce:\n", a3);
    print_memory(&mm);
    printf("Fragments: %d\n", count_fragments(&mm));
    printf("Allocated: %d / %d\n", mm.allocated_size, mm.total_size);

    return 0;
}
