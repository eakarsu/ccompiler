int printf(const char *fmt, ...);

struct Block {
    int start;
    int size;
    int used;
    int alloc_id;
};

struct MemManager {
    struct Block blocks[64];
    int block_count;
    int total_size;
    int next_id;
};

void mm_init(struct MemManager *mm, int size) {
    mm->block_count = 1;
    mm->blocks[0].start = 0;
    mm->blocks[0].size = size;
    mm->blocks[0].used = 0;
    mm->blocks[0].alloc_id = 0;
    mm->total_size = size;
    mm->next_id = 1;
}

int mm_alloc(struct MemManager *mm, int size) {
    int i = 0;
    while (i < mm->block_count) {
        if (!mm->blocks[i].used && mm->blocks[i].size >= size) {
            int remaining = mm->blocks[i].size - size;
            mm->blocks[i].used = 1;
            mm->blocks[i].alloc_id = mm->next_id;
            mm->next_id = mm->next_id + 1;
            if (remaining > 0) {
                int j = mm->block_count;
                while (j > i + 1) {
                    mm->blocks[j] = mm->blocks[j - 1];
                    j = j - 1;
                }
                mm->blocks[i + 1].start = mm->blocks[i].start + size;
                mm->blocks[i + 1].size = remaining;
                mm->blocks[i + 1].used = 0;
                mm->blocks[i + 1].alloc_id = 0;
                mm->blocks[i].size = size;
                mm->block_count = mm->block_count + 1;
            }
            return mm->blocks[i].alloc_id;
        }
        i = i + 1;
    }
    return -1;
}

int mm_free(struct MemManager *mm, int alloc_id) {
    int i = 0;
    while (i < mm->block_count) {
        if (mm->blocks[i].used && mm->blocks[i].alloc_id == alloc_id) {
            mm->blocks[i].used = 0;
            mm->blocks[i].alloc_id = 0;
            if (i + 1 < mm->block_count && !mm->blocks[i + 1].used) {
                mm->blocks[i].size = mm->blocks[i].size + mm->blocks[i + 1].size;
                int j = i + 1;
                while (j < mm->block_count - 1) {
                    mm->blocks[j] = mm->blocks[j + 1];
                    j = j + 1;
                }
                mm->block_count = mm->block_count - 1;
            }
            if (i > 0 && !mm->blocks[i - 1].used) {
                mm->blocks[i - 1].size = mm->blocks[i - 1].size + mm->blocks[i].size;
                int j = i;
                while (j < mm->block_count - 1) {
                    mm->blocks[j] = mm->blocks[j + 1];
                    j = j + 1;
                }
                mm->block_count = mm->block_count - 1;
            }
            return 1;
        }
        i = i + 1;
    }
    return 0;
}

int mm_used_bytes(struct MemManager *mm) {
    int total = 0;
    int i = 0;
    while (i < mm->block_count) {
        if (mm->blocks[i].used) {
            total = total + mm->blocks[i].size;
        }
        i = i + 1;
    }
    return total;
}

int mm_free_bytes(struct MemManager *mm) {
    int total = 0;
    int i = 0;
    while (i < mm->block_count) {
        if (!mm->blocks[i].used) {
            total = total + mm->blocks[i].size;
        }
        i = i + 1;
    }
    return total;
}

int mm_largest_free(struct MemManager *mm) {
    int mx = 0;
    int i = 0;
    while (i < mm->block_count) {
        if (!mm->blocks[i].used && mm->blocks[i].size > mx) {
            mx = mm->blocks[i].size;
        }
        i = i + 1;
    }
    return mx;
}

int mm_fragment_count(struct MemManager *mm) {
    int c = 0;
    int i = 0;
    while (i < mm->block_count) {
        if (!mm->blocks[i].used) c = c + 1;
        i = i + 1;
    }
    return c;
}

int mm_alloc_count(struct MemManager *mm) {
    int c = 0;
    int i = 0;
    while (i < mm->block_count) {
        if (mm->blocks[i].used) c = c + 1;
        i = i + 1;
    }
    return c;
}

int mm_find_alloc_start(struct MemManager *mm, int alloc_id) {
    int i = 0;
    while (i < mm->block_count) {
        if (mm->blocks[i].used && mm->blocks[i].alloc_id == alloc_id) {
            return mm->blocks[i].start;
        }
        i = i + 1;
    }
    return -1;
}

int main(void) {
    struct MemManager mm;
    mm_init(&mm, 1000);

    // EXPECT: init_free=1000
    printf("init_free=%d\n", mm_free_bytes(&mm));

    // EXPECT: init_used=0
    printf("init_used=%d\n", mm_used_bytes(&mm));

    int a1 = mm_alloc(&mm, 200);
    // EXPECT: alloc1=1
    printf("alloc1=%d\n", a1);

    int a2 = mm_alloc(&mm, 300);
    // EXPECT: alloc2=2
    printf("alloc2=%d\n", a2);

    int a3 = mm_alloc(&mm, 100);
    // EXPECT: alloc3=3
    printf("alloc3=%d\n", a3);

    // EXPECT: used=600
    printf("used=%d\n", mm_used_bytes(&mm));

    // EXPECT: free=400
    printf("free=%d\n", mm_free_bytes(&mm));

    // EXPECT: allocs=3
    printf("allocs=%d\n", mm_alloc_count(&mm));

    // EXPECT: blocks=4
    printf("blocks=%d\n", mm.block_count);

    // EXPECT: a1_start=0
    printf("a1_start=%d\n", mm_find_alloc_start(&mm, a1));

    // EXPECT: a2_start=200
    printf("a2_start=%d\n", mm_find_alloc_start(&mm, a2));

    mm_free(&mm, a2);
    // EXPECT: free_after=700
    printf("free_after=%d\n", mm_free_bytes(&mm));

    // EXPECT: frags=2
    printf("frags=%d\n", mm_fragment_count(&mm));

    // EXPECT: largest_free=400
    printf("largest_free=%d\n", mm_largest_free(&mm));

    int a4 = mm_alloc(&mm, 250);
    // EXPECT: alloc4=4
    printf("alloc4=%d\n", a4);

    // EXPECT: a4_start=200
    printf("a4_start=%d\n", mm_find_alloc_start(&mm, a4));

    mm_free(&mm, a1);
    mm_free(&mm, a3);
    mm_free(&mm, a4);

    // EXPECT: final_blocks=1
    printf("final_blocks=%d\n", mm.block_count);

    // EXPECT: final_free=1000
    printf("final_free=%d\n", mm_free_bytes(&mm));

    int fail = mm_alloc(&mm, 1500);
    // EXPECT: too_big=-1
    printf("too_big=%d\n", fail);

    return 0;
}
