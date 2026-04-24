int printf(const char *fmt, ...);

int set_data[4][64];
int set_used[4][64];
int set_size[4];

int hash_int(int x, int mod) {
    if (x < 0) x = -x;
    return x % mod;
}

void set_init(int id) {
    set_size[id] = 0;
    int i;
    for (i = 0; i < 64; i = i + 1) {
        set_used[id][i] = 0;
        set_data[id][i] = 0;
    }
}

int set_add(int id, int val) {
    int h = hash_int(val, 64);
    int i;
    for (i = 0; i < 64; i = i + 1) {
        int idx = (h + i) % 64;
        if (set_used[id][idx] == 0) {
            set_data[id][idx] = val;
            set_used[id][idx] = 1;
            set_size[id] = set_size[id] + 1;
            return 1;
        }
        if (set_data[id][idx] == val) {
            return 0;
        }
    }
    return -1;
}

int set_contains(int id, int val) {
    int h = hash_int(val, 64);
    int i;
    for (i = 0; i < 64; i = i + 1) {
        int idx = (h + i) % 64;
        if (set_used[id][idx] == 0) return 0;
        if (set_data[id][idx] == val) return 1;
    }
    return 0;
}

int set_remove(int id, int val) {
    int h = hash_int(val, 64);
    int i;
    for (i = 0; i < 64; i = i + 1) {
        int idx = (h + i) % 64;
        if (set_used[id][idx] == 0) return 0;
        if (set_data[id][idx] == val) {
            set_used[id][idx] = 0;
            set_size[id] = set_size[id] - 1;
            return 1;
        }
    }
    return 0;
}

void set_union(int a, int b, int out) {
    set_init(out);
    int i;
    for (i = 0; i < 64; i = i + 1) {
        if (set_used[a][i]) set_add(out, set_data[a][i]);
    }
    for (i = 0; i < 64; i = i + 1) {
        if (set_used[b][i]) set_add(out, set_data[b][i]);
    }
}

void set_intersect(int a, int b, int out) {
    set_init(out);
    int i;
    for (i = 0; i < 64; i = i + 1) {
        if (set_used[a][i]) {
            if (set_contains(b, set_data[a][i])) {
                set_add(out, set_data[a][i]);
            }
        }
    }
}

void set_difference(int a, int b, int out) {
    set_init(out);
    int i;
    for (i = 0; i < 64; i = i + 1) {
        if (set_used[a][i]) {
            if (set_contains(b, set_data[a][i]) == 0) {
                set_add(out, set_data[a][i]);
            }
        }
    }
}

int main(void) {
    set_init(0);
    set_init(1);

    // EXPECT: empty set size: 0
    printf("empty set size: %d\n", set_size[0]);

    set_add(0, 10);
    set_add(0, 20);
    set_add(0, 30);
    // EXPECT: set0 size: 3
    printf("set0 size: %d\n", set_size[0]);

    // EXPECT: contains 10: 1
    printf("contains 10: %d\n", set_contains(0, 10));

    // EXPECT: contains 20: 1
    printf("contains 20: %d\n", set_contains(0, 20));

    // EXPECT: contains 99: 0
    printf("contains 99: %d\n", set_contains(0, 99));

    // EXPECT: dup add returns 0: 0
    int r = set_add(0, 10);
    printf("dup add returns 0: %d\n", r);

    // EXPECT: size after dup: 3
    printf("size after dup: %d\n", set_size[0]);

    // EXPECT: remove 20: 1
    r = set_remove(0, 20);
    printf("remove 20: %d\n", r);

    // EXPECT: size after remove: 2
    printf("size after remove: %d\n", set_size[0]);

    // EXPECT: 20 gone: 0
    printf("20 gone: %d\n", set_contains(0, 20));

    // EXPECT: remove missing: 0
    r = set_remove(0, 99);
    printf("remove missing: %d\n", r);

    set_add(0, 20);
    set_add(1, 20);
    set_add(1, 30);
    set_add(1, 40);
    set_add(1, 50);

    set_union(0, 1, 2);
    // EXPECT: union size: 5
    printf("union size: %d\n", set_size[2]);

    // EXPECT: union has 10: 1
    printf("union has 10: %d\n", set_contains(2, 10));

    // EXPECT: union has 50: 1
    printf("union has 50: %d\n", set_contains(2, 50));

    set_intersect(0, 1, 3);
    // EXPECT: intersect size: 2
    printf("intersect size: %d\n", set_size[3]);

    // EXPECT: intersect has 20: 1
    printf("intersect has 20: %d\n", set_contains(3, 20));

    // EXPECT: intersect has 30: 1
    printf("intersect has 30: %d\n", set_contains(3, 30));

    // EXPECT: intersect not has 10: 0
    printf("intersect not has 10: %d\n", set_contains(3, 10));

    set_difference(0, 1, 2);
    // EXPECT: diff size: 1
    printf("diff size: %d\n", set_size[2]);

    // EXPECT: diff has 10: 1
    printf("diff has 10: %d\n", set_contains(2, 10));

    // EXPECT: done
    printf("done\n");

    return 0;
}
