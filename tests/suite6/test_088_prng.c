int printf(const char *fmt, ...);

int lcg_next(int state) {
    return (state * 1103515 + 12345) & 32767;
}

int lcg_range(int state, int lo, int hi) {
    int range = hi - lo + 1;
    int val = state % range;
    if (val < 0) val = val + range;
    return lo + val;
}

void generate_sequence(int seed, int *out, int len) {
    int state = seed;
    int i;
    for (i = 0; i < len; i = i + 1) {
        state = lcg_next(state);
        out[i] = state;
    }
}

void shuffle(int *arr, int len, int seed) {
    int state = seed;
    int i;
    for (i = len - 1; i > 0; i = i - 1) {
        state = lcg_next(state);
        int j = (state % (i + 1));
        if (j < 0) j = j + (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

int all_in_range(int *arr, int len, int lo, int hi) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] < lo || arr[i] > hi) return 0;
    }
    return 1;
}

int contains(int *arr, int len, int val) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] == val) return 1;
    }
    return 0;
}

int is_permutation(int *arr, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (!contains(arr, len, i)) return 0;
    }
    return 1;
}

int main(void) {
    int s0 = lcg_next(0);
    // EXPECT: LCG(0): 12345
    printf("LCG(0): %d\n", s0);

    int s1 = lcg_next(1);
    // EXPECT: LCG(1): 1748
    printf("LCG(1): %d\n", s1 & 32767);

    int s2 = lcg_next(s0);
    // EXPECT: LCG(LCG(0)): 2236
    printf("LCG(LCG(0)): %d\n", s2);

    int seq[8];
    generate_sequence(42, seq, 8);
    // EXPECT: Seq[0]: 26023
    printf("Seq[0]: %d\n", seq[0]);
    // EXPECT: Seq[1]: 22102
    printf("Seq[1]: %d\n", seq[1]);
    // EXPECT: Seq[2]: 23115
    printf("Seq[2]: %d\n", seq[2]);

    int r0 = lcg_range(12345, 0, 9);
    // EXPECT: Range 12345 [0,9]: 5
    printf("Range 12345 [0,9]: %d\n", r0);

    int r1 = lcg_range(100, 1, 6);
    // EXPECT: Range 100 [1,6]: 5
    printf("Range 100 [1,6]: %d\n", r1);

    int ranged[10];
    int state = 42;
    int i;
    for (i = 0; i < 10; i = i + 1) {
        state = lcg_next(state);
        ranged[i] = lcg_range(state, 0, 99);
    }
    // EXPECT: All in range: 1
    printf("All in range: %d\n", all_in_range(ranged, 10, 0, 99));

    int same_seed_1[4];
    int same_seed_2[4];
    generate_sequence(123, same_seed_1, 4);
    generate_sequence(123, same_seed_2, 4);
    int same = 1;
    for (i = 0; i < 4; i = i + 1) {
        if (same_seed_1[i] != same_seed_2[i]) same = 0;
    }
    // EXPECT: Same seed same seq: 1
    printf("Same seed same seq: %d\n", same);

    int diff_seed_1[4];
    int diff_seed_2[4];
    generate_sequence(1, diff_seed_1, 4);
    generate_sequence(2, diff_seed_2, 4);
    int diff = 0;
    for (i = 0; i < 4; i = i + 1) {
        if (diff_seed_1[i] != diff_seed_2[i]) diff = 1;
    }
    // EXPECT: Diff seed diff seq: 1
    printf("Diff seed diff seq: %d\n", diff);

    int perm[6];
    for (i = 0; i < 6; i = i + 1) perm[i] = i;
    shuffle(perm, 6, 42);
    // EXPECT: Shuffled is permutation: 1
    printf("Shuffled is permutation: %d\n", is_permutation(perm, 6));

    int ordered = 1;
    for (i = 0; i < 6; i = i + 1) {
        if (perm[i] != i) ordered = 0;
    }
    // EXPECT: Shuffled not identity: 1
    printf("Shuffled not identity: %d\n", ordered == 0 ? 1 : 0);

    // EXPECT: Perm[0]: 0
    printf("Perm[0]: %d\n", perm[0]);
    // EXPECT: Perm[1]: 4
    printf("Perm[1]: %d\n", perm[1]);

    int perm2[6];
    for (i = 0; i < 6; i = i + 1) perm2[i] = i;
    shuffle(perm2, 6, 99);
    int diff_shuffle = 0;
    for (i = 0; i < 6; i = i + 1) {
        if (perm[i] != perm2[i]) diff_shuffle = 1;
    }
    // EXPECT: Diff seed diff shuffle: 1
    printf("Diff seed diff shuffle: %d\n", diff_shuffle);

    return 0;
}
