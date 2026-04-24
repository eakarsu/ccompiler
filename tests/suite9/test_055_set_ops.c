int printf(const char *fmt, ...);

// Set operations using hash-based integer sets
// Supports union, intersection, difference

int set_data[4][64];
int set_used[4][64];
int set_cap;

void set_init(int sid) {
    int i;
    set_cap = 64;
    for (i = 0; i < set_cap; i++) {
        set_data[sid][i] = 0;
        set_used[sid][i] = 0;
    }
}

int set_hash(int val) {
    int h;
    h = val;
    if (h < 0) h = -h;
    return h % 64;
}

int set_add(int sid, int val) {
    int idx;
    int start;
    idx = set_hash(val);
    start = idx;
    while (1) {
        if (set_used[sid][idx] == 0) {
            set_data[sid][idx] = val;
            set_used[sid][idx] = 1;
            return 1;
        }
        if (set_data[sid][idx] == val) {
            return 0;  // already present
        }
        idx = (idx + 1) % 64;
        if (idx == start) return 0;
    }
    return 0;
}

int set_contains(int sid, int val) {
    int idx;
    int start;
    idx = set_hash(val);
    start = idx;
    while (1) {
        if (set_used[sid][idx] == 0) return 0;
        if (set_data[sid][idx] == val) return 1;
        idx = (idx + 1) % 64;
        if (idx == start) return 0;
    }
    return 0;
}

int set_size(int sid) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < 64; i++) {
        if (set_used[sid][i]) c = c + 1;
    }
    return c;
}

// Union of set a and set b into set dst
void set_union(int dst, int a, int b) {
    int i;
    set_init(dst);
    for (i = 0; i < 64; i++) {
        if (set_used[a][i]) {
            set_add(dst, set_data[a][i]);
        }
    }
    for (i = 0; i < 64; i++) {
        if (set_used[b][i]) {
            set_add(dst, set_data[b][i]);
        }
    }
}

// Intersection of set a and set b into set dst
void set_intersect(int dst, int a, int b) {
    int i;
    set_init(dst);
    for (i = 0; i < 64; i++) {
        if (set_used[a][i]) {
            if (set_contains(b, set_data[a][i])) {
                set_add(dst, set_data[a][i]);
            }
        }
    }
}

// Difference: elements in a but not in b, into set dst
void set_diff(int dst, int a, int b) {
    int i;
    set_init(dst);
    for (i = 0; i < 64; i++) {
        if (set_used[a][i]) {
            if (set_contains(b, set_data[a][i]) == 0) {
                set_add(dst, set_data[a][i]);
            }
        }
    }
}

int set_equal(int a, int b) {
    int i;
    if (set_size(a) != set_size(b)) return 0;
    for (i = 0; i < 64; i++) {
        if (set_used[a][i]) {
            if (set_contains(b, set_data[a][i]) == 0) return 0;
        }
    }
    return 1;
}

int set_subset(int a, int b) {
    int i;
    for (i = 0; i < 64; i++) {
        if (set_used[a][i]) {
            if (set_contains(b, set_data[a][i]) == 0) return 0;
        }
    }
    return 1;
}

int main(void) {
    // Set 0: {1, 2, 3, 4, 5}
    set_init(0);
    set_add(0, 1); set_add(0, 2); set_add(0, 3); set_add(0, 4); set_add(0, 5);
    // EXPECT: set0 size=5
    printf("set0 size=%d\n", set_size(0));

    // Set 1: {3, 4, 5, 6, 7}
    set_init(1);
    set_add(1, 3); set_add(1, 4); set_add(1, 5); set_add(1, 6); set_add(1, 7);
    // EXPECT: set1 size=5
    printf("set1 size=%d\n", set_size(1));

    // Union into set 2
    set_union(2, 0, 1);
    // EXPECT: union size=7
    printf("union size=%d\n", set_size(2));
    // EXPECT: union has 1=1
    printf("union has 1=%d\n", set_contains(2, 1));
    // EXPECT: union has 7=1
    printf("union has 7=%d\n", set_contains(2, 7));
    // EXPECT: union has 9=0
    printf("union has 9=%d\n", set_contains(2, 9));

    // Intersection into set 2
    set_intersect(2, 0, 1);
    // EXPECT: intersect size=3
    printf("intersect size=%d\n", set_size(2));
    // EXPECT: intersect has 3=1
    printf("intersect has 3=%d\n", set_contains(2, 3));
    // EXPECT: intersect has 5=1
    printf("intersect has 5=%d\n", set_contains(2, 5));
    // EXPECT: intersect has 1=0
    printf("intersect has 1=%d\n", set_contains(2, 1));

    // Difference (0 - 1) into set 2
    set_diff(2, 0, 1);
    // EXPECT: diff 0-1 size=2
    printf("diff 0-1 size=%d\n", set_size(2));
    // EXPECT: diff has 1=1
    printf("diff has 1=%d\n", set_contains(2, 1));
    // EXPECT: diff has 2=1
    printf("diff has 2=%d\n", set_contains(2, 2));
    // EXPECT: diff has 3=0
    printf("diff has 3=%d\n", set_contains(2, 3));

    // Difference (1 - 0) into set 3
    set_diff(3, 1, 0);
    // EXPECT: diff 1-0 size=2
    printf("diff 1-0 size=%d\n", set_size(3));
    // EXPECT: diff10 has 6=1
    printf("diff10 has 6=%d\n", set_contains(3, 6));
    // EXPECT: diff10 has 7=1
    printf("diff10 has 7=%d\n", set_contains(3, 7));

    // Duplicate add should not increase size
    set_add(0, 1);
    set_add(0, 2);
    // EXPECT: no dup size=5
    printf("no dup size=%d\n", set_size(0));

    // Subset check
    set_init(2);
    set_add(2, 3); set_add(2, 4);
    // EXPECT: subset 2 of 0=1
    printf("subset 2 of 0=%d\n", set_subset(2, 0));
    // EXPECT: subset 0 of 2=0
    printf("subset 0 of 2=%d\n", set_subset(0, 2));

    // Equal check
    set_init(3);
    set_add(3, 1); set_add(3, 2); set_add(3, 3); set_add(3, 4); set_add(3, 5);
    // EXPECT: equal 0 3=1
    printf("equal 0 3=%d\n", set_equal(0, 3));

    // EXPECT: done
    printf("done\n");

    return 0;
}
