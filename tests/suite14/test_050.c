int printf(const char *fmt, ...);

// Cellular automaton: Rule 30 and Rule 110
// 1D elementary cellular automaton

// A rule maps a 3-bit neighborhood (left, center, right) to a new value
// Rule number is an 8-bit value where bit i = output for pattern i

int apply_rule(int rule, int left, int center, int right) {
    int pattern = (left << 2) | (center << 1) | right;
    return (rule >> pattern) & 1;
}

void ca_step(int *cur, int *next, int len, int rule) {
    int i = 0;
    while (i < len) {
        int left;
        int right;

        if (i == 0) left = 0;
        else left = cur[i - 1];

        if (i == len - 1) right = 0;
        else right = cur[i + 1];

        next[i] = apply_rule(rule, left, cur[i], right);
        i = i + 1;
    }
}

void copy_array(int *dst, int *src, int len) {
    int i = 0;
    while (i < len) {
        dst[i] = src[i];
        i = i + 1;
    }
}

void clear_array(int *arr, int len) {
    int i = 0;
    while (i < len) {
        arr[i] = 0;
        i = i + 1;
    }
}

void print_row(int *arr, int len) {
    int i = 0;
    while (i < len) {
        printf("%d", arr[i]);
        i = i + 1;
    }
    printf("\n");
}

int count_ones(int *arr, int len) {
    int count = 0;
    int i = 0;
    while (i < len) {
        if (arr[i] == 1) count = count + 1;
        i = i + 1;
    }
    return count;
}

int main(void) {
    int cur[21];
    int next[21];
    int width = 21;
    int i;
    int gen;

    // === Rule 30 Tests ===
    // Rule 30 in binary: 00011110
    // Pattern: 111->0 110->0 101->0 100->1 011->1 010->1 001->1 000->0

    // Test 1: Apply rule 30 to individual patterns
    printf("t1: %d%d%d%d%d%d%d%d\n",
        apply_rule(30, 1, 1, 1),  // pattern 7 -> bit 7 of 30 = 0
        apply_rule(30, 1, 1, 0),  // pattern 6 -> bit 6 of 30 = 0
        apply_rule(30, 1, 0, 1),  // pattern 5 -> bit 5 of 30 = 0
        apply_rule(30, 1, 0, 0),  // pattern 4 -> bit 4 of 30 = 1
        apply_rule(30, 0, 1, 1),  // pattern 3 -> bit 3 of 30 = 1
        apply_rule(30, 0, 1, 0),  // pattern 2 -> bit 2 of 30 = 1
        apply_rule(30, 0, 0, 1),  // pattern 1 -> bit 1 of 30 = 1
        apply_rule(30, 0, 0, 0)); // pattern 0 -> bit 0 of 30 = 0
    // EXPECT: t1: 00011110

    // Test 2: Rule 30, single center cell, generation 0 (width=11)
    int w11 = 11;
    int c11[11];
    int n11[11];
    clear_array(c11, w11);
    c11[5] = 1; // center cell on
    print_row(c11, w11);
    // EXPECT: 00000100000

    // Test 3: Rule 30, generation 1
    ca_step(c11, n11, w11, 30);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00001110000

    // Test 4: Rule 30, generation 2
    ca_step(c11, n11, w11, 30);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00011001000

    // Test 5: Rule 30, generation 3
    ca_step(c11, n11, w11, 30);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00110111100

    // Test 6: Rule 30, generation 4
    ca_step(c11, n11, w11, 30);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 01100100010

    // Test 7: Count ones after 4 generations of Rule 30
    printf("t7: ones=%d\n", count_ones(c11, w11));
    // EXPECT: t7: ones=4

    // === Rule 110 Tests ===
    // Rule 110 in binary: 01101110
    // Pattern: 111->0 110->1 101->1 100->0 011->1 010->1 001->1 000->0

    // Test 8: Apply rule 110 to individual patterns
    printf("t8: %d%d%d%d%d%d%d%d\n",
        apply_rule(110, 1, 1, 1),  // pattern 7 -> 0
        apply_rule(110, 1, 1, 0),  // pattern 6 -> 1
        apply_rule(110, 1, 0, 1),  // pattern 5 -> 1
        apply_rule(110, 1, 0, 0),  // pattern 4 -> 0
        apply_rule(110, 0, 1, 1),  // pattern 3 -> 1
        apply_rule(110, 0, 1, 0),  // pattern 2 -> 1
        apply_rule(110, 0, 0, 1),  // pattern 1 -> 1
        apply_rule(110, 0, 0, 0)); // pattern 0 -> 0
    // EXPECT: t8: 01101110

    // Test 9: Rule 110, single center cell, gen 0 (width=11)
    clear_array(c11, w11);
    c11[5] = 1;
    print_row(c11, w11);
    // EXPECT: 00000100000

    // Test 10: Rule 110, generation 1
    ca_step(c11, n11, w11, 110);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00001100000

    // Test 11: Rule 110, generation 2
    ca_step(c11, n11, w11, 110);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00011100000

    // Test 12: Rule 110, generation 3
    ca_step(c11, n11, w11, 110);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00110100000

    // Test 13: Rule 110, generation 4
    ca_step(c11, n11, w11, 110);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 01111100000

    // Test 14: Count ones after 4 gens of Rule 110
    printf("t14: ones=%d\n", count_ones(c11, w11));
    // EXPECT: t14: ones=5

    // === Rule 90 (Sierpinski triangle) ===
    // Rule 90 = XOR of left and right neighbors

    // Test 15: Rule 90 pattern check
    printf("t15: %d%d%d%d%d%d%d%d\n",
        apply_rule(90, 1, 1, 1),  // 7 -> 0
        apply_rule(90, 1, 1, 0),  // 6 -> 1
        apply_rule(90, 1, 0, 1),  // 5 -> 0
        apply_rule(90, 1, 0, 0),  // 4 -> 1
        apply_rule(90, 0, 1, 1),  // 3 -> 1
        apply_rule(90, 0, 1, 0),  // 2 -> 0
        apply_rule(90, 0, 0, 1),  // 1 -> 1
        apply_rule(90, 0, 0, 0)); // 0 -> 0
    // EXPECT: t15: 01011010

    // Test 16: Rule 90 from single center cell (width=11)
    clear_array(c11, w11);
    c11[5] = 1;
    // Gen 1
    ca_step(c11, n11, w11, 90);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00001010000

    // Gen 2
    ca_step(c11, n11, w11, 90);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00010001000

    // Gen 3
    ca_step(c11, n11, w11, 90);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00101010100

    // Test 17: Count population over multiple generations of Rule 30
    // Start with single cell, run 6 generations, count total ones
    int w15 = 15;
    int c15[15];
    int n15[15];
    clear_array(c15, w15);
    c15[7] = 1;
    int total_ones = count_ones(c15, w15);
    gen = 0;
    while (gen < 6) {
        ca_step(c15, n15, w15, 30);
        copy_array(c15, n15, w15);
        total_ones = total_ones + count_ones(c15, w15);
        gen = gen + 1;
    }
    printf("t17: total=%d\n", total_ones);
    // EXPECT: t17: total=31

    // Test 18: Two initial cells
    clear_array(c11, w11);
    c11[4] = 1;
    c11[6] = 1;
    ca_step(c11, n11, w11, 30);
    copy_array(c11, n11, w11);
    print_row(c11, w11);
    // EXPECT: 00011011000

    printf("cellular automaton done\n");
    // EXPECT: cellular automaton done

    return 0;
}
