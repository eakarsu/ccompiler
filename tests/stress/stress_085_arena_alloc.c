// stress_085_arena_alloc.c - Arena allocator from static char array
// EXPECT: arena_init=0
// EXPECT: alloc1_ok=1
// EXPECT: alloc2_ok=1
// EXPECT: used_after_2=12
// EXPECT: write_read_ok=1
// EXPECT: aligned_8_ok=1
// EXPECT: aligned_used=16
// EXPECT: struct_alloc_ok=1
// EXPECT: struct_vals=42 99
// EXPECT: array_alloc_ok=1
// EXPECT: array_vals=0 1 2 3 4 5 6 7 8 9
// EXPECT: many_allocs=100
// EXPECT: reset_used=0
// EXPECT: reuse_ok=1
// EXPECT: fill_remaining=1
// EXPECT: overflow_null=1
// EXPECT: multi_arena_ok=1
// EXPECT: linked_list_ok=10 9 8 7 6 5 4 3 2 1
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// --- Arena allocator ---
struct Arena {
    char *buffer;
    int capacity;
    int used;
};

void arena_init(struct Arena *a, char *buf, int cap) {
    a->buffer = buf;
    a->capacity = cap;
    a->used = 0;
}

void arena_reset(struct Arena *a) {
    a->used = 0;
}

int arena_used(struct Arena *a) {
    return a->used;
}

int arena_remaining(struct Arena *a) {
    return a->capacity - a->used;
}

// Align offset up to alignment (must be power of 2)
int align_up(int offset, int alignment) {
    return (offset + alignment - 1) & ~(alignment - 1);
}

void *arena_alloc(struct Arena *a, int size, int alignment) {
    int aligned_offset = align_up(a->used, alignment);
    if (aligned_offset + size > a->capacity) {
        return (void *)0;
    }
    void *ptr = (void *)(a->buffer + aligned_offset);
    a->used = aligned_offset + size;
    return ptr;
}

// Convenience: allocate with default alignment of 4
void *arena_alloc4(struct Arena *a, int size) {
    return arena_alloc(a, size, 4);
}

// --- Test structs ---
struct Point {
    int x;
    int y;
};

struct Node {
    int value;
    struct Node *next;
};

// Static buffers
char buf1[4096];
char buf2[4096];
char buf3[512];

int main(void) {
    struct Arena arena;
    arena_init(&arena, buf1, 4096);
    printf("arena_init=%d\n", arena_used(&arena));

    // Basic allocations
    int *p1 = (int *)arena_alloc4(&arena, 4);
    int ok1 = (p1 != (void *)0) ? 1 : 0;
    printf("alloc1_ok=%d\n", ok1);

    int *p2 = (int *)arena_alloc4(&arena, 8);
    int ok2 = (p2 != (void *)0) ? 1 : 0;
    printf("alloc2_ok=%d\n", ok2);

    printf("used_after_2=%d\n", arena_used(&arena));

    // Write and read back
    *p1 = 12345;
    p2[0] = 67890;
    p2[1] = 11111;
    int wr_ok = (*p1 == 12345 && p2[0] == 67890 && p2[1] == 11111) ? 1 : 0;
    printf("write_read_ok=%d\n", wr_ok);

    // Aligned allocation (8-byte)
    arena_reset(&arena);
    int *a1 = (int *)arena_alloc(&arena, 4, 4);
    *a1 = 1;
    // Next alloc at 8-byte alignment
    long *a2 = (long *)arena_alloc(&arena, 8, 8);
    // Check that a2 is 8-byte aligned
    long addr = (long)a2;
    int aligned_ok = ((addr % 8) == 0) ? 1 : 0;
    printf("aligned_8_ok=%d\n", aligned_ok);
    // used: 4 -> align to 8 -> +8 = 16, but we had 4 bytes first aligned to 8
    // Actually: alloc 4 at align 4 -> used=4. alloc 8 at align 8 -> aligned_offset=8, used=16
    // Wait, let's see: after first alloc, used=4. align_up(4,8)=8. 8+8=16. so used=16.
    // But I printed 24 above. Let me recount.
    // Hmm, arena was reset, so used=0. alloc(4,4): align_up(0,4)=0, used=4.
    // alloc(8,8): align_up(4,8)=8, used=8+8=16.
    printf("aligned_used=%d\n", arena_used(&arena));

    // Allocate a struct
    arena_reset(&arena);
    struct Point *pt = (struct Point *)arena_alloc4(&arena, sizeof(struct Point));
    int sok = (pt != (void *)0) ? 1 : 0;
    printf("struct_alloc_ok=%d\n", sok);
    pt->x = 42;
    pt->y = 99;
    printf("struct_vals=%d %d\n", pt->x, pt->y);

    // Allocate an array of 10 ints
    int *arr = (int *)arena_alloc4(&arena, 10 * sizeof(int));
    int arr_ok = (arr != (void *)0) ? 1 : 0;
    printf("array_alloc_ok=%d\n", arr_ok);
    int i;
    for (i = 0; i < 10; i++) {
        arr[i] = i;
    }
    printf("array_vals=");
    for (i = 0; i < 10; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");

    // Many small allocations
    arena_reset(&arena);
    int count = 0;
    for (i = 0; i < 100; i++) {
        int *p = (int *)arena_alloc4(&arena, 4);
        if (p != (void *)0) {
            *p = i;
            count++;
        }
    }
    printf("many_allocs=%d\n", count);

    // Reset and verify
    arena_reset(&arena);
    printf("reset_used=%d\n", arena_used(&arena));

    // Reuse after reset
    int *reuse = (int *)arena_alloc4(&arena, 4);
    *reuse = 777;
    int reuse_ok = (*reuse == 777) ? 1 : 0;
    printf("reuse_ok=%d\n", reuse_ok);

    // Fill remaining space
    arena_reset(&arena);
    int remaining = arena_remaining(&arena);
    char *big = (char *)arena_alloc(&arena, remaining, 1);
    int fill_ok = (big != (void *)0 && arena_remaining(&arena) == 0) ? 1 : 0;
    printf("fill_remaining=%d\n", fill_ok);

    // Overflow detection
    void *overflow = arena_alloc4(&arena, 1);
    int ov_ok = (overflow == (void *)0) ? 1 : 0;
    printf("overflow_null=%d\n", ov_ok);

    // Multiple arenas
    struct Arena arena2;
    arena_init(&arena2, buf2, 4096);
    int *ma1 = (int *)arena_alloc4(&arena, 4); // arena is full, should fail
    int *ma2 = (int *)arena_alloc4(&arena2, 4); // arena2 should work
    int multi_ok = (ma1 == (void *)0 && ma2 != (void *)0) ? 1 : 0;
    printf("multi_arena_ok=%d\n", multi_ok);

    // Build a linked list in an arena
    struct Arena arena3;
    arena_init(&arena3, buf3, 512);
    struct Node *head = (void *)0;
    for (i = 1; i <= 10; i++) {
        struct Node *n = (struct Node *)arena_alloc(&arena3, sizeof(struct Node), 8);
        n->value = i;
        n->next = head;
        head = n;
    }
    // Print linked list (should be 10, 9, 8, ..., 1)
    printf("linked_list_ok=");
    struct Node *cur = head;
    int first = 1;
    while (cur != (void *)0) {
        if (!first) printf(" ");
        printf("%d", cur->value);
        first = 0;
        cur = cur->next;
    }
    printf("\n");

    printf("pass_done=1\n");
    return 0;
}
