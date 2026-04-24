int printf(const char *fmt, ...);
// EXPECT: Initial:\n  [ 0-63] free (64)\n  Used: 0, Free: 64, Blocks: 1\nAfter 3 allocs (addr 0, 10, 30):\n  [ 0- 9] proc_A (10)\n  [10-29] proc_B (20)\n  [30-44] proc_C (15)\n  [45-63] free (19)\n  Used: 45, Free: 19, Blocks: 4\nAfter freeing proc_B:\n  [ 0- 9] proc_A (10)\n  [10-29] free (20)\n  [30-44] proc_C (15)\n  [45-63] free (19)\n  Used: 25, Free: 39, Blocks: 4\nAlloc 8 into gap (addr 10):\n  [ 0- 9] proc_A (10)\n  [10-17] proc_D (8)\n  [18-29] free (12)\n  [30-44] proc_C (15)\n  [45-63] free (19)\n  Used: 33, Free: 31, Blocks: 5\nAfter freeing proc_A and proc_D:\n  [ 0-29] free (30)\n  [30-44] proc_C (15)\n  [45-63] free (19)\n  Used: 15, Free: 49, Blocks: 3\nAlloc 5 (addr 0):\n  [ 0- 4] proc_E (5)\n  [ 5-29] free (25)\n  [30-44] proc_C (15)\n  [45-63] free (19)\n  Used: 20, Free: 44, Blocks: 4

#define MEM_SIZE 64
#define MAX_BLOCKS 10

struct MemBlock {
    int start;
    int size;
    int used;
    char *label;
};

struct MemManager {
    struct MemBlock blocks[MAX_BLOCKS];
    int num_blocks;
    int total_size;
};

void mm_init(struct MemManager *mm) {
    mm->num_blocks = 1;
    mm->blocks[0].start = 0;
    mm->blocks[0].size = MEM_SIZE;
    mm->blocks[0].used = 0;
    mm->blocks[0].label = "free";
    mm->total_size = MEM_SIZE;
}

int mm_alloc(struct MemManager *mm, int size, char *label) {
    int i;
    for (i = 0; i < mm->num_blocks; i++) {
        if (!mm->blocks[i].used && mm->blocks[i].size >= size) {
            if (mm->blocks[i].size > size && mm->num_blocks < MAX_BLOCKS) {
                int j;
                for (j = mm->num_blocks; j > i + 1; j--) {
                    mm->blocks[j] = mm->blocks[j - 1];
                }
                mm->blocks[i + 1].start = mm->blocks[i].start + size;
                mm->blocks[i + 1].size = mm->blocks[i].size - size;
                mm->blocks[i + 1].used = 0;
                mm->blocks[i + 1].label = "free";
                mm->num_blocks++;
            }
            mm->blocks[i].size = size;
            mm->blocks[i].used = 1;
            mm->blocks[i].label = label;
            return mm->blocks[i].start;
        }
    }
    return -1;
}

int mm_free(struct MemManager *mm, int addr) {
    int i;
    for (i = 0; i < mm->num_blocks; i++) {
        if (mm->blocks[i].used && mm->blocks[i].start == addr) {
            mm->blocks[i].used = 0;
            mm->blocks[i].label = "free";
            /* merge with next free block */
            if (i + 1 < mm->num_blocks && !mm->blocks[i + 1].used) {
                mm->blocks[i].size = mm->blocks[i].size + mm->blocks[i + 1].size;
                int j;
                for (j = i + 1; j < mm->num_blocks - 1; j++) {
                    mm->blocks[j] = mm->blocks[j + 1];
                }
                mm->num_blocks--;
            }
            /* merge with previous free block */
            if (i > 0 && !mm->blocks[i - 1].used) {
                mm->blocks[i - 1].size = mm->blocks[i - 1].size + mm->blocks[i].size;
                int j;
                for (j = i; j < mm->num_blocks - 1; j++) {
                    mm->blocks[j] = mm->blocks[j + 1];
                }
                mm->num_blocks--;
            }
            return 0;
        }
    }
    return -1;
}

void mm_print(struct MemManager *mm) {
    int i;
    int used_total = 0;
    int free_total = 0;
    for (i = 0; i < mm->num_blocks; i++) {
        printf("  [%2d-%2d] %s (%d)\n",
               mm->blocks[i].start,
               mm->blocks[i].start + mm->blocks[i].size - 1,
               mm->blocks[i].label, mm->blocks[i].size);
        if (mm->blocks[i].used) used_total = used_total + mm->blocks[i].size;
        else free_total = free_total + mm->blocks[i].size;
    }
    printf("  Used: %d, Free: %d, Blocks: %d\n", used_total, free_total, mm->num_blocks);
}

int main(void) {
    struct MemManager mm;
    mm_init(&mm);
    printf("Initial:\n");
    mm_print(&mm);

    int a1 = mm_alloc(&mm, 10, "proc_A");
    int a2 = mm_alloc(&mm, 20, "proc_B");
    int a3 = mm_alloc(&mm, 15, "proc_C");
    printf("After 3 allocs (addr %d, %d, %d):\n", a1, a2, a3);
    mm_print(&mm);

    mm_free(&mm, a2);
    printf("After freeing proc_B:\n");
    mm_print(&mm);

    int a4 = mm_alloc(&mm, 8, "proc_D");
    printf("Alloc 8 into gap (addr %d):\n", a4);
    mm_print(&mm);

    mm_free(&mm, a1);
    mm_free(&mm, a4);
    printf("After freeing proc_A and proc_D:\n");
    mm_print(&mm);

    int a5 = mm_alloc(&mm, 5, "proc_E");
    printf("Alloc 5 (addr %d):\n", a5);
    mm_print(&mm);

    return 0;
}
