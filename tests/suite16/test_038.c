int printf(const char *fmt, ...);

/* Stack-based Memory Allocator (Arena with Save/Restore)
 * A linear allocator with save points. You can "push" a save point
 * and "pop" back to it, freeing everything allocated after the save.
 * Useful for temporary allocations in a game frame, etc.
 */

int arena[256];
int arena_top;
int arena_capacity;

/* Save point stack */
int save_stack[16];
int save_top;

/* Stats */
int peak_usage;
int total_alloc_count;

void arena_init(int cap) {
    arena_top = 0;
    arena_capacity = cap;
    save_top = 0;
    peak_usage = 0;
    total_alloc_count = 0;
    int i;
    for (i = 0; i < 256; i++) arena[i] = 0;
    for (i = 0; i < 16; i++) save_stack[i] = 0;
}

int arena_alloc(int size) {
    if (arena_top + size > arena_capacity) return -1;
    int addr = arena_top;
    arena_top = arena_top + size;
    total_alloc_count = total_alloc_count + 1;
    if (arena_top > peak_usage) peak_usage = arena_top;
    return addr;
}

void arena_save(void) {
    if (save_top < 16) {
        save_stack[save_top] = arena_top;
        save_top = save_top + 1;
    }
}

void arena_restore(void) {
    if (save_top > 0) {
        save_top = save_top - 1;
        arena_top = save_stack[save_top];
    }
}

int arena_used(void) { return arena_top; }
int arena_free(void) { return arena_capacity - arena_top; }

void arena_write(int addr, int val) {
    if (addr >= 0 && addr < 256) arena[addr] = val;
}

int arena_read(int addr) {
    if (addr >= 0 && addr < 256) return arena[addr];
    return -1;
}

void test_basic(void) {
    printf("=== Arena Basic ===\n"); // EXPECT: === Arena Basic ===
    arena_init(64);

    int a = arena_alloc(4);
    printf("alloc 4: %d used=%d\n", a, arena_used()); // EXPECT: alloc 4: 0 used=4

    arena_write(a, 100);
    arena_write(a + 1, 200);
    printf("read: %d %d\n", arena_read(a), arena_read(a + 1)); // EXPECT: read: 100 200

    int b = arena_alloc(8);
    printf("alloc 8: %d used=%d\n", b, arena_used()); // EXPECT: alloc 8: 4 used=12

    printf("free=%d\n", arena_free()); // EXPECT: free=52
}

void test_save_restore(void) {
    printf("=== Save/Restore ===\n"); // EXPECT: === Save/Restore ===
    arena_init(64);

    int a = arena_alloc(10);
    arena_write(a, 42);
    printf("before save: used=%d\n", arena_used()); // EXPECT: before save: used=10

    arena_save();

    int b = arena_alloc(20);
    arena_write(b, 99);
    printf("after temp alloc: used=%d\n", arena_used()); // EXPECT: after temp alloc: used=30

    arena_restore();
    printf("after restore: used=%d\n", arena_used()); // EXPECT: after restore: used=10
    printf("original data preserved: %d\n", arena_read(a)); // EXPECT: original data preserved: 42
}

void test_nested_saves(void) {
    printf("=== Nested Saves ===\n"); // EXPECT: === Nested Saves ===
    arena_init(100);

    int a = arena_alloc(5);
    arena_write(a, 1);
    printf("level 0: used=%d\n", arena_used()); // EXPECT: level 0: used=5

    arena_save(); /* save point 1 */
    int b = arena_alloc(10);
    arena_write(b, 2);
    printf("level 1: used=%d\n", arena_used()); // EXPECT: level 1: used=15

    arena_save(); /* save point 2 */
    int c = arena_alloc(20);
    arena_write(c, 3);
    printf("level 2: used=%d\n", arena_used()); // EXPECT: level 2: used=35

    arena_save(); /* save point 3 */
    int d = arena_alloc(15);
    arena_write(d, 4);
    printf("level 3: used=%d\n", arena_used()); // EXPECT: level 3: used=50

    arena_restore(); /* back to save point 3 */
    printf("restore to level 2: used=%d\n", arena_used()); // EXPECT: restore to level 2: used=35

    arena_restore(); /* back to save point 2 */
    printf("restore to level 1: used=%d\n", arena_used()); // EXPECT: restore to level 1: used=15
    printf("level 1 data: %d\n", arena_read(b)); // EXPECT: level 1 data: 2

    arena_restore(); /* back to save point 1 */
    printf("restore to level 0: used=%d\n", arena_used()); // EXPECT: restore to level 0: used=5
    printf("level 0 data: %d\n", arena_read(a)); // EXPECT: level 0 data: 1
}

void test_game_frame_pattern(void) {
    printf("=== Game Frame Pattern ===\n"); // EXPECT: === Game Frame Pattern ===
    arena_init(200);

    /* Permanent allocations */
    int player = arena_alloc(4);
    arena_write(player, 100); /* health */
    arena_write(player + 1, 50); /* x */
    arena_write(player + 2, 30); /* y */
    arena_write(player + 3, 0); /* score */

    int frame;
    for (frame = 0; frame < 5; frame++) {
        arena_save();

        /* Frame-temporary allocations */
        int temp = arena_alloc(10);
        arena_write(temp, frame * 10);

        /* Update score */
        int score = arena_read(player + 3) + 10;
        arena_write(player + 3, score);

        arena_restore(); /* free frame temps */
    }

    printf("health=%d score=%d\n", arena_read(player), arena_read(player + 3)); // EXPECT: health=100 score=50
    printf("used=%d peak=%d\n", arena_used(), peak_usage); // EXPECT: used=4 peak=14
    printf("alloc count=%d\n", total_alloc_count); // EXPECT: alloc count=6
}

void test_overflow(void) {
    printf("=== Overflow Protection ===\n"); // EXPECT: === Overflow Protection ===
    arena_init(20);

    int a = arena_alloc(15);
    printf("alloc 15: %d\n", a); // EXPECT: alloc 15: 0

    int b = arena_alloc(10);
    printf("alloc 10 (should fail): %d\n", b); // EXPECT: alloc 10 (should fail): -1

    printf("used=%d\n", arena_used()); // EXPECT: used=15

    arena_save();
    int c = arena_alloc(5);
    printf("alloc 5: %d\n", c); // EXPECT: alloc 5: 15

    int d = arena_alloc(1);
    printf("alloc 1 (should fail): %d\n", d); // EXPECT: alloc 1 (should fail): -1

    arena_restore();
    printf("after restore: used=%d\n", arena_used()); // EXPECT: after restore: used=15
}

void test_interleaved(void) {
    printf("=== Interleaved Operations ===\n"); // EXPECT: === Interleaved Operations ===
    arena_init(128);

    int sum = 0;
    int i;
    for (i = 0; i < 8; i++) {
        arena_save();
        int addr = arena_alloc(4);
        arena_write(addr, (i + 1) * (i + 1));
        sum = sum + arena_read(addr);
        arena_restore();
    }
    printf("sum of squares 1-8: %d\n", sum); // EXPECT: sum of squares 1-8: 204
    printf("final used: %d\n", arena_used()); // EXPECT: final used: 0
}

int main(void) {
    test_basic();
    test_save_restore();
    test_nested_saves();
    test_game_frame_pattern();
    test_overflow();
    test_interleaved();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
